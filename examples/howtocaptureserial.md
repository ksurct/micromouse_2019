Use the following command to capture serial input in linux subsystem for windows

```bash
$ sudo chmod 666 /dev/ttyS7
$ stty -F /dev/ttyS7 sane 115200
$ cat < /dev/ttyS7 > data.txt
```