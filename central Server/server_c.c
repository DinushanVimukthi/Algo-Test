#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./json/json.h"
#include <microhttpd.h>

#define PORT 5000

// In-memory storage for host metrics
struct HostMetric {
    char *hostname;
    char *data;
    struct HostMetric *next;
};

struct HostMetric *host_metrics = NULL;

void add_metric(const char *hostname, const char *data) {
    struct HostMetric *new_metric = malloc(sizeof(struct HostMetric));
    new_metric->hostname = strdup(hostname);
    new_metric->data = strdup(data);
    new_metric->next = host_metrics;
    host_metrics = new_metric;
}

char *get_all_metrics_json() {
    struct json_object *json = json_object_new_object();
    struct HostMetric *current = host_metrics;

    while (current) {
        struct json_object *data = json_tokener_parse(current->data);
        json_object_object_add(json, current->hostname, data);
        current = current->next;
    }

    const char *result = json_object_to_json_string(json);
    char *response = strdup(result);
    json_object_put(json); // Free the JSON object
    return response;
}

int request_handler(void *cls, struct MHD_Connection *connection,
                    const char *url, const char *method,
                    const char *version, const char *upload_data,
                    size_t *upload_data_size, void **con_cls) {
    struct MHD_Response *response;
    int ret;

    if (strcmp(url, "/report") == 0 && strcmp(method, "POST") == 0) {
        static char *received_data = NULL;
        if (*upload_data_size > 0) {
            received_data = realloc(received_data, *upload_data_size + 1);
            strncpy(received_data, upload_data, *upload_data_size);
            received_data[*upload_data_size] = '\0';
            *upload_data_size = 0;
            return MHD_YES;
        }

        struct json_object *json = json_tokener_parse(received_data);
        if (json) {
            struct json_object *hostname;
            if (json_object_object_get_ex(json, "hostname", &hostname)) {
                add_metric(json_object_get_string(hostname), received_data);
                json_object_put(json); // Free the JSON object
                free(received_data);
                response = MHD_create_response_from_buffer(strlen("Metrics received"),
                                                           (void *)"Metrics received",
                                                           MHD_RESPMEM_PERSISTENT);
                ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
                MHD_destroy_response(response);
                return ret;
            }
            json_object_put(json); // Free the JSON object
        }

        response = MHD_create_response_from_buffer(strlen("Hostname missing"),
                                                   (void *)"Hostname missing",
                                                   MHD_RESPMEM_PERSISTENT);
        ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
        MHD_destroy_response(response);
        return ret;
    }

    if (strcmp(url, "/hosts") == 0 && strcmp(method, "GET") == 0) {
        char *response_data = get_all_metrics_json();
        response = MHD_create_response_from_buffer(strlen(response_data),
                                                   (void *)response_data,
                                                   MHD_RESPMEM_MUST_FREE);
        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        return ret;
    }

    response = MHD_create_response_from_buffer(strlen("Not Found"),
                                               (void *)"Not Found",
                                               MHD_RESPMEM_PERSISTENT);
    ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

int main() {
    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                              &request_handler, NULL, MHD_OPTION_END);
    if (!daemon) {
        fprintf(stderr, "Failed to start server\n");
        return 1;
    }

    printf("Server running on port %d\n", PORT);
    getchar(); // Wait for user input to stop the server

    MHD_stop_daemon(daemon);
    return 0;
}
