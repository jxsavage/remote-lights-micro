# Remote Lights Microcontroller

Microcontroller code for the teensy3.6 remote lights project.

## Platform.io setup
[Platform.io systemd daemon setup reference](https://community.platformio.org/t/howto-remote-agent-systemd-service/1739/4)
### Edit /etc/systemd/system/pio-remote.service
`
[Unit]
Description=Run platformio remote agent
Requires=network-online.target
After=network-online.target

[Service]
Type=simple
Environment="PLATFORMIO_AUTH_TOKEN=<insert your token here>"
WorkingDirectory=/home/<your user name>/.platformio/penv
ExecStart=/home/<your user name>/.platformio/penv/bin/pio remote agent start --serve-in-foreground
User=<your user name>
Restart=always
RestartSec=3

[Install]
WantedBy=multi-user.target
`
### Run commands:
`sudo systemctl daemon-reload && sudo systemctl enable pio-remote`
**then...**
`sudo systemctl start pio-remote`
**check status with...**
`sudo systemctl status pio-remote`
