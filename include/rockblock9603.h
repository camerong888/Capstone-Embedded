/*
To open up serial connection on MAC:
/dev/cu.DEVICE
screen dev/cu.DEVICE 19200

AT Commands to send a basic message:

Test Connection: AT
RESPONSE: OK

Check Signal Strength: AT+CSQ
RESPONSE: +CSQ:0-5\n\nOK

Send the Message: AT+SBDWT=TEXT MESSAGE
Response: OK

Initiate Sending of Message: AT+SBDIX
Response: +SBDIX: 0, 1, 0, 0, 0, 0

OK

*/ 
