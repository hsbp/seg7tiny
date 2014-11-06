void setup() {
	Serial1.begin(1200);
	Spark.function("txserial", txSerial);
}

int txSerial(String args) {
	for (int pos = 0; pos < args.length() / 2; pos++) {
		byte b = (charHex(args[pos * 2]) << 4) | charHex(args[pos * 2 + 1]);
		Serial1.write(b);
	}
}

byte charHex(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 0x0A;
	if (c >= 'a' && c <= 'f') return c - 'a' + 0x0a;
	return 0;
}

void loop() {
}
