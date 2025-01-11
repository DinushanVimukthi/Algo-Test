# Install python3
sudo apt-get install python3 -y

# Install pip3
sudo apt-get install python3-pip -y

# Install requirement.txt packages
pip3 install -r requirement.txt

# Install crontab
# (crontab -l 2>/dev/null; echo "*/5 * * * * /bin/bash /home/ubuntu/Observer/monitor_script.sh") | crontab -

# Run the script
python3 /home/ubuntu/Observer/monitor.py