# cat > php_shell.sh <<CONTENT
  #!/bin/sh
  /sbin/service sshd restart
cat /root/secret
