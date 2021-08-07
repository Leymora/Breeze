#include "zipManager.h"

zipManager::zipManager() { }

void zipManager::unZip(unsigned char*& contents, int& stSize, std::string zipFile, std::string file)
{
	int zipError = 0;
	zip* z = zip_open(zipFile.c_str(), 0, &zipError);

	const char* name = file.c_str();
	struct zip_stat st;
	zip_stat_init(&st);
	zip_stat(z, name, 0, &st);

	contents = new unsigned char[st.size];

	zip_file* f = zip_fopen(z, name, 0);
	if (f != NULL)
	{
		zip_fread(f, contents, st.size);
		zip_fclose(f);
		zip_close(z);
	}
	stSize = st.size;
}
