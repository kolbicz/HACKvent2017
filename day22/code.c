int main ( int argc, char **argv ) {
	
	if(argc == 1){
		return -1;
	}
	
	int i;
	long lSize;
	unsigned char secret[] = { 0x69,0x63,0x65,0x2d,0x63,0x6f,0x6c,0x64 };	// "ice-cold"
	unsigned char *hint="frozen water";
	FILE *keyfile = fopen(argv[1], "rb");	
	fseek (keyfile , 0 , SEEK_END);
  	lSize = ftell (keyfile);
  	rewind (keyfile);	
	unsigned char in[lSize], out[lSize];
	fread(in, lSize, 1, keyfile);
	fclose (keyfile);
	
	ICE_KEY *key = ice_key_create(1);
	ice_key_set(key, secret);
	
	for (i = 0; i < lSize; i += 8) {
		ice_key_encrypt(key, in + i, out + i);
    }
    
    FILE *flag = fopen ("HV17-flag", "wb");
    fwrite(out, lSize, 1, flag);
    fclose (flag);

	return 0;
}