import libvirt

def get_vm_count():
    try:
        conn = libvirt.open('qemu:///system')  # Connect to the QEMU system
        if conn is None:
            print("Failed to open connection to qemu:///system")
            return

        # Get the number of active and inactive VMs
        active_vms = conn.listDomainsID()  # Active VMs (IDs)
        inactive_vms = conn.listDefinedDomains()  # Inactive VMs (names)

        print(f"Active VMs: {len(active_vms)}")
        print(f"Inactive VMs: {len(inactive_vms)}")
        print(f"Total VMs: {len(active_vms) + len(inactive_vms)}")
        print(f"Get Total CPU Map: {conn.getCPUMap()}")

        conn.close()
    except libvirt.libvirtError as e:
        print(f"Failed to get VM details: {e}")

if __name__ == "__main__":
    get_vm_count()
