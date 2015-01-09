mms-access
==========

The Raspberry pi based gateway for the access components.

Requirements
------------

* Raspian
* pySerial - `apt-get install python-serial`
* pyYaml - `apt-get install python-yaml`

Setup
-----

* Clone from github.com
* Disable serial console on serial port ( http://elinux.org/RPi_Serial_Connection)
* Copy the example config file to `/etc/mms/config.yaml` and modify.
* Add the following to rc.local before the `exit 0`:
```sh
# Starting the TinkerMill MMS Gateway daemon
python /root/mms-access/raspi/testServer.py &
```
