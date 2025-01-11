from flask import Flask, request, jsonify

app = Flask(__name__)

# Store metrics in memory for simplicity (use a database in production)
host_metrics = {}

@app.route('/report', methods=['POST'])
def report():
    data = request.json
    hostname = data.get("hostname")
    if hostname:
        host_metrics[hostname] = data
        return jsonify({"message": "Metrics received"}), 200
    return jsonify({"error": "Hostname missing"}), 400

@app.route('/hosts', methods=['GET'])
def get_hosts():
    return jsonify(host_metrics), 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
