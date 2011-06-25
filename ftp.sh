#!/bin/sh
USERNAME="FTP-USERNAME"
PASSWORD="FTP-PASSWORD"
SERVER="FTP-SERVER"
 
# local directory to pickup *.tar.gz file
mv topic.tmp topic.xml
FILE="topic.xml"
 
# remote server directory to upload backup
BACKUPDIR="/var/www"
 
# login to remote server
ftp -q -n $SERVER <<EOF
$PASSWORD
cd $BACKUPDIR
mput $FILE
quit
EOF
