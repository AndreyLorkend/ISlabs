#pragma once
#include <windows.h>
#include <string>
#include <array>
#include "../cryptopp/gost.h"
#include "../cryptopp/modes.h"
#include "../cryptopp/files.h"
#include "../cryptopp/osrng.h"

using gost_key_t = std::array<CryptoPP::byte, CryptoPP::GOST::DEFAULT_KEYLENGTH>;
using gost_iv_t = std::array<CryptoPP::byte, CryptoPP::GOST::BLOCKSIZE>;

class Window
{
public:
	virtual void printArray(COORD outCoord) = 0;
	virtual void fillArray() = 0;
	virtual HANDLE getHandleStdOut() = 0;
	virtual void setExitFlag(bool flag) = 0;
	virtual void handleArrayData(COORD outCoord) = 0;
	virtual void encodePolybius() = 0;
	virtual void decodePolybius() = 0;
	virtual void encodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out) = 0;
	virtual void decodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out) = 0;

	virtual gost_key_t getGostKey() = 0;
	virtual gost_iv_t getGostIV() = 0;

	virtual std::string getFilenameIn() = 0;
	virtual std::string getFileNameOut() = 0;

	virtual ~Window();
};

