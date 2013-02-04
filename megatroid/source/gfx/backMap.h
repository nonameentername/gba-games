const unsigned char backMap[20][30]={
0x02, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x03, 0x04, 0x00, 
0x00, 0x04, 0x00, 0x01, 0x04, 0x00, 0x00, 0x01, 0x04, 0x00, 0x04, 0x02, 0x05, 0x05, 0x03, 0x04, 
0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x04, 0x00, 0x01, 0x04, 0x00, 0x04, 0x0B, 0x00, 0x00, 0x0B, 
0x06, 0x07, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x0B, 0x00, 0x06, 
0x07, 0x0A, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0B, 0x0B, 0x02, 0x03, 0x0B, 0x00, 0x01, 
0x0B, 0x02, 0x00, 0x01, 0x04, 0x00, 0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 0x00, 0x00, 0x01, 0x04, 
0x02, 0x05, 0x0B, 0x00, 0x01, 0x0B, 0x02, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 
0x06, 0x07, 0x0A, 0x00, 0x0B, 0x02, 0x05, 0x05, 0x03, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x00, 0x00, 
0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x04, 0x00, 
0x01, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x0B, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0B, 0x00, 
0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 
0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 
0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x00, 0x01, 0x04, 0x00, 0x0B, 0x02, 
0x05, 0x05, 0x03, 0x0B, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0B, 
0x0B, 0x02, 0x03, 0x0B, 0x06, 0x07, 0x0B, 0x02, 0x06, 0x07, 0x0A, 0x00, 0x0B, 0x00, 0x01, 0x00, 
0x01, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x02, 0x05, 0x0B, 0x06, 0x07, 0x0B, 0x02, 0x0B, 0x0B, 0x00, 
0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x04, 0x00, 0x01, 0x00, 0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 
0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 
0x00, 0x01, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 0x0B, 0x02, 0x05, 0x05, 0x03, 0x0B, 0x00, 0x0A, 
0x06, 0x07, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 
0x0B, 0x00, 0x00, 0x01, 0x04, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x0B, 0x00, 0x00, 0x01, 0x04, 
0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 
0x06, 0x07, 0x0A, 0x00, 0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x00, 0x00, 
0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x02, 0x03, 0x0B, 0x06, 0x07, 0x0B, 0x02, 0x04, 0x00, 
0x01, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x0B, 0x00, 0x04, 0x00, 0x01, 0x02, 0x05, 0x0B, 0x06, 
0x07, 0x0B, 0x02, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 
0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 
0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x00, 0x01, 0x04, 0x00, 0x0B, 0x00, 
0x01, 0x00, 0x01, 0x0B, 0x00, 0x00, 0x01, 0x04, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 
0x0B, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x00, 0x0B, 0x06, 0x07, 0x06, 
0x07, 0x0B, 0x00, 0x06, 0x07, 0x0A, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 
0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x04, 0x00, 0x01, 0x00, 0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 
0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x0B, 0x00, 0x0B, 0x0B, 0x00, 0x00, 0x0B, 
0x06, 0x07, 0x0B, 0x00, 0x0A, 0x06, 0x07, 0x00, 0x0B, 0x06, 0x07, 0x06, 0x07, 0x0B, 0x00, 0x0A, 
0x06, 0x07, 0x00, 0x00, 0x0B, 0x06, 0x07, 0x0B, 0x00, 0x0B, 0x02, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 
0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x03, };