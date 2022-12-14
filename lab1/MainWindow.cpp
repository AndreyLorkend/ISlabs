#include "MainWindow.h"
#include <fstream>
#include <vector>

int MainWindow::getIndex(std::unordered_set< char> S,  char K)
{
    int index = 0;
    for (auto u : S) {
        if (u == K)
            return index;
        index++;
    }
    return -1;
}

void MainWindow::fillArray()
{
    srand(time(0));
    for (int i = 0; i < ARR_LENGTH; i++) {
        arr[i] = rand() % 21 - 10;
    }
}

void MainWindow::fillAsciiTable()
{
    srand(seed);
    while (asciiTable.size() < 256) {
         char symbol = rand() % 256;
        asciiTable.insert(symbol);
    }
    std::unordered_set< char>::iterator startSymbol = asciiTable.begin();
    std::unordered_set< char>::iterator endSymbol = asciiTable.end();

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            asciiTableArr[i][j] = *startSymbol;
            startSymbol++;
        }
    }
}

void MainWindow::generateKey()
{
    srand(seed);
    while (keySet.size() < 16) {
        char symbol = rand() % 256;
        keySet.insert(symbol);
    }

    std::unordered_set< char>::iterator startSymbol = keySet.begin();

    for (int i = 0; i < 16; i++) {
        keyArr[i] = *startSymbol;
        startSymbol++;
    }
}

MainWindow::MainWindow()
{
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    isExitFlagSet = false;
    fdwMode = 0;

    CryptoPP::AutoSeededRandomPool rng;
    rng.GenerateBlock(gostKey.data(), gostKey.size());
    rng.GenerateBlock(gostIV.data(), gostIV.size());

    kb = 0;
      
    filenameIn = "otchet.pdf";
    filenameOut = "encoded_otcher.bin";
}

void MainWindow::initLRC()
{
    LRC1.reset();
    LRC2.reset();
    LRC1 |= 68;	 
    LRC2 |= 35; 
}

void MainWindow::printArray(COORD outCoord)
{
    DWORD cWritten = 0;
    std::string outText;
    std::list<CHAR*>textLines;
    char txt[10];
    for (int i = 0; i < ARR_LENGTH; i++) {
        wsprintf(txt, "  %d", arr[i]);
        outText.append(txt);
    }

    textLines.push_back(outText.data());

    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("INITIAL_DATA", TEXT_BOX_ELEMENT, textLines, hStdout);
    }
}

VOID MainWindow::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
    switch (mer.dwEventFlags)
    {
    case 0:
        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            for (auto &[key, value] : winBoxes) {
                value->checkClick(mer, this);
            }
        }
        break;
    default:
        break;
    }
}

VOID MainWindow::SetConsoleAppearance(WORD wAttributes)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiScreenInfo;
    COORD coordStart = { 0, 0 };
    DWORD dwNumWritten = 0;
    DWORD dwScrSize;

    if (!GetConsoleScreenBufferInfo(hStdout, &csbiScreenInfo))
        ErrorExit((LPSTR)"GetConsoleScreenBufferInfo");
    dwScrSize = csbiScreenInfo.dwMaximumWindowSize.X * csbiScreenInfo.dwMaximumWindowSize.Y;
    FillConsoleOutputAttribute(hStdout, wAttributes, dwScrSize, coordStart, &dwNumWritten);
    SetConsoleTextAttribute(hStdout, wAttributes);
}

VOID MainWindow::DrawInterface()
{
    // Adding LEGEND
    std::list<CHAR*> textLines;
    CHAR outText1[64];
    CHAR outText2[64];
    CHAR outText3[64];
    CHAR outText4[64];
    CHAR outText5[64];

    wsprintf(outText1, "This program includes three types of cypher. ");
    textLines.push_back(outText1);

    wsprintf(outText2, "1) Polybius Square.");
    textLines.push_back(outText2);

    wsprintf(outText3, "2) GOST 28147-89 cypher with CFB mode. ");
    textLines.push_back(outText3);

    wsprintf(outText4, "3) Stream cypher.");
    textLines.push_back(outText4);

    wsprintf(outText5, "The program was developed by Andrew Derendyaev B19-781-1.");
    textLines.push_back(outText5);

    // Drawing another elements
    for (auto& [key, value] : winBoxes) {
        value->drawElements(hStdout);
    }

    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("LEGEND", TEXT_BOX_ELEMENT, textLines, hStdout);
    }

    textLines.clear();
}

void MainWindow::handleConsole()
{
    DWORD cNumRead = 0;
    while (!isExitFlagSet) {
        if (!ReadConsoleInput(hStdin, irInBuf, 128, &cNumRead)) {
            ErrorExit((LPSTR)"ReadConsoleInput");
        }

        for (int i = 0; i < cNumRead; i++)
        {
            switch (irInBuf[i].EventType)
            {
            case MOUSE_EVENT: // mouse input
                MouseEventProc(irInBuf[i].Event.MouseEvent);
                break;
            default:
                break;
            }
        }
    }
}

bool MainWindow::SetConsoleParams()
{
    bool errorFlag = false;
    WORD attr = BACKGROUND_BLUE | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    SetConsoleAppearance(attr);

    if (hStdin == INVALID_HANDLE_VALUE) {
        errorFlag = ErrorExit((LPSTR)"GetStdHandle");
    }

    if (!GetConsoleMode(hStdin, &fdwSaveOldMode)) {
        errorFlag = ErrorExit((LPSTR)"GetConsoleMode");
    }

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
    if (!SetConsoleMode(hStdin, fdwMode)) {
        errorFlag = ErrorExit((LPSTR)"SetConsoleMode");
    }

    return errorFlag;
}

bool MainWindow::ErrorExit(LPSTR lpszMessage)
{
    fprintf(stderr, "%s\n", lpszMessage);
    SetConsoleMode(hStdin, fdwSaveOldMode);
    return true;
}

void MainWindow::handleArrayData(COORD outCoord)
{
    std::list<CHAR*> textLines;
    CHAR outText1[32];
    CHAR outText2[32];

    wsprintf(outText1, "Negative numbers count: %d", findNegativeCount(ARR_LENGTH, arr));
    textLines.push_back(outText1);

    wsprintf(outText2, "Positive avrage arithmetic: %d", findPositiveArithmeticMean(ARR_LENGTH, arr));
    textLines.push_back(outText2);


    WinElement* element;
    for (auto& [key, value] : winBoxes) {
        value->handleWinElement("HANDLED_DATA", TEXT_BOX_ELEMENT, textLines, hStdout);
    }
    textLines.clear();
}

void MainWindow::encodePolybius()
{
    setCursor({ 2, 13 });
    std::cout << "Enter the key: ";
    std::cin >> seed;
    fillAsciiTable();
    generateKey();
    resetCursor();

    setCursor({2, 13});
    std::string fileName = "";
    std::cout << "Enter the filename you want to encode: ";
    std::cin.clear();
    std::cin.ignore();
    std::cin >> fileName;

    std::fstream dataFile(fileName, std::ios_base::in | std::ios_base::binary);
    std::fstream writeFile("encoded_" + fileName, std::ios::binary | std::ios::out);
    //std::fstream dbg("1dbg.txt", std::ios::binary | std::ios::out);
    char temp;
    int column = 0;
    int row = 0;
    char rowSymbol;
    char columnSymbol;
    std::unordered_set<char>::iterator curentSymbol = keySet.begin();

    while (dataFile.read(&temp, sizeof( char))) {
        bool stop = false;
        for (int i = 0; i < 16 && !stop; i++) {
            for (int j = 0; j < 16 && !stop; j++) {
                if (asciiTableArr[i][j] == temp) {
                    writeFile.write((char*)&keyArr[i], sizeof(char));
                    writeFile.write((char*)&keyArr[j], sizeof(char));
                    stop = true;
                }
            }
        }

       // column = getIndex(asciiTable, static_cast<char>(temp)) % 16;
       // row = getIndex(asciiTable, static_cast<char>(temp)) >> 4;


       //std::unordered_set<char>::iterator ch = keySet.find(*curentSymbol);
       //for (int i = 0; i < row; i++) {
       //    ch++;
       //}

       //char c = *ch;
       ///*std::cout << c;*/
       //writeFile.write((char*)&c, sizeof(char));

       //ch = keySet.find(*curentSymbol);
       //for (int i = 0; i < column; i++) {
       //    ch++;
       //}
       //c = *ch;
       //writeFile.write((char*)&c, sizeof(char));
    }

    resetCursor();
    //dbg.close();
    dataFile.close();
    writeFile.close();
    DrawInterface();
    
}

void MainWindow::decodePolybius()
{
    char temp[2];
    int column = 0;
    int row = 0;
    int index = 0;

    std::unordered_set<char>::iterator startSymbol = asciiTable.begin();

    
    setCursor({ 2, 13 });
    std::cout << "Enter the key: ";
    std::cin >> seed;
    fillAsciiTable();
    generateKey();
    resetCursor();
    setCursor({ 2, 13 });
    std::string fileName = "";
    std::cout << "Enter the filename you want to decode: ";
    std::cin >> fileName;

    std::fstream dataFile(fileName, std::ios_base::in | std::ios_base::binary);
    std::fstream writeFile("decoded_" + fileName, std::ios::binary | std::ios::out);

    char buffer[40];
    int bufferIndex = 0;
    std::fill(&buffer[0], &buffer[40], 0);

    while (!dataFile.eof()) {
        dataFile.read(temp, 2 * sizeof(char));
        for (int i = 0; i < 16; i++) {
            if (keyArr[i] == temp[0]) {
                row = i;
                break;
            }
        }

        for (int i = 0; i < 16; i++) {
            if (keyArr[i] == temp[1]) {
                column = i;
                break;
            }
        }

        buffer[bufferIndex] = asciiTableArr[row][column];
        bufferIndex++;

        if (bufferIndex == 40 || dataFile.eof()) {
            if (bufferIndex != 40 && dataFile.eof()) {
                bufferIndex--;
            }
            writeFile.write((char*)&buffer, sizeof(char) * bufferIndex);
            bufferIndex = 0;
            std::fill(&buffer[0], &buffer[40], 0);
        }

        //row = getIndex(keySet, static_cast<char>(temp[0]));
        //column = getIndex(keySet, static_cast<char>(temp[1]));


        //index = (row * 16) + (column);

        //std::unordered_set< char>::iterator ch = asciiTable.find(*startSymbol);
        //for (int i = 0; i < index; i++) {
        //    ch++;
        //}

        ////std::cout << row << " " << column << " : " << index << "\n";
        //std::cout << (char)*ch;
        //writeFile.write((char*)&ch, sizeof(char));
    }

    //resetCursor();
    dataFile.close();
    writeFile.close();
    DrawInterface();
}

void MainWindow::encodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out)
{
    std::ofstream writeFileKey("key.bin", std::ios::binary);
    for (int i = 0; i < key.size(); i++)
    {
        writeFileKey << key.data()[i];
    }
    writeFileKey.close();

    std::ofstream writeFileIV("iv.bin", std::ios::binary);
    for (int i = 0; i < iv.size(); i++)
    {
        writeFileIV << iv.data()[i];
    }
    writeFileIV.close();

    setCursor({ 2, 13 });
    std::string fileName;
    std::cout << "Enter the filename you want to encode: ";
    std::cin >> fileName;

    CryptoPP::CFB_Mode<CryptoPP::GOST>::Encryption cipher{};
    cipher.SetKeyWithIV(key.data(), key.size(), iv.data());
    std::cout << key.size() << std::endl;
    std::cout << iv.size() << std::endl;

    std::ifstream in{ fileName, std::ios::binary};
    std::ofstream out{ "encoded_" + fileName, std::ios::binary};

    CryptoPP::FileSource{ in, /*pumpAll=*/true,
                         new CryptoPP::StreamTransformationFilter{
                             cipher, new CryptoPP::FileSink{out}} };
    //filenameIn = "encoded_otcher.bin";
    //filenameOut = "decoded_otchet.pdf";
    resetCursor();
    in.close();
    out.close();
    DrawInterface();
}

void MainWindow::decodeGOST(const gost_key_t& key, const gost_iv_t& iv, const std::string& filename_in, const std::string& filename_out)
{
    std::ifstream keyFile("key.bin", std::ios_base::binary);
    std::vector<byte> keyGOST;
    byte t;
    while (keyFile >> t)
        keyGOST.push_back(t);
    keyFile.close();

    std::ifstream ivFile("iv.bin", std::ios_base::binary);
    std::vector<byte> ivGOST;
    byte t_v;
    while (ivFile >> t_v)
        ivGOST.push_back(t_v);
    ivFile.close();
    
    std::cout << keyGOST.size() << std::endl;
    std::cout << ivGOST.size() << std::endl;

    setCursor({ 2, 13 });
    std::string fileName = "";
    std::cout << "Enter the filename you want to decode: ";
    std::cin >> fileName;

    CryptoPP::CFB_Mode<CryptoPP::GOST>::Decryption cipher{};
    cipher.SetKeyWithIV(keyGOST.data(), keyGOST.size(), ivGOST.data());

    std::ifstream in{ fileName, std::ios::binary };
    std::ofstream out{ "decoded_" + fileName, std::ios::binary};

    CryptoPP::FileSource{ in, /*pumpAll=*/true,
                         new CryptoPP::StreamTransformationFilter{
                             cipher, new CryptoPP::FileSink{out}} };
    resetCursor();
    in.close();
    out.close();
    DrawInterface();
}

gost_key_t MainWindow::getGostKey()
{
    return gostKey;
}

gost_iv_t MainWindow::getGostIV()
{
    return gostIV;
}

std::string MainWindow::getFilenameIn()
{
    return filenameIn;
}

std::string MainWindow::getFileNameOut()
{
    return filenameOut;
}

void MainWindow::Show()
{
    if (!SetConsoleParams()) {
        CreateElements();
        DrawInterface();
        handleConsole();
    }

    SetConsoleMode(hStdin, fdwSaveOldMode);
    system("cls");
}

void MainWindow::encodeGenerator()
{
    DrawInterface();
    initLRC();
    kb = 0;
    setCursor({ 2, 13 });
    std::string fileName;
    std::cout << "Enter the filename you want to encode: ";
    std::cin >> fileName;

    std::ifstream inFile(fileName, std::ios_base::binary);
    std::ofstream outFile("encoded_"+ fileName, std::ios_base::binary | std::ios_base::trunc);

    setCursor({ 2, 13 });
    DrawInterface();
    std::cout << "Шифрование файла..." << std::endl;
    while (!inFile.eof())
    {
        inFile.read(buf, 16);
        if (kb >= 128)
        {
            for (int i = 0; i < inFile.gcount(); i++)
            {
                buf[i] = buf[i] ^ generatorLRC();
            }
        }
        kb++;
        outFile.write(buf, inFile.gcount());
    }
    std::cout << "Файл зашифрован!" << std::endl;
    inFile.close(); 
    outFile.close();
    resetCursor();
}

void MainWindow::decodeGenerator()
{
    DrawInterface();
    initLRC();
    kb = 0;    

    setCursor({ 2, 13 });
    std::string fileName;
    std::cout << "Enter the filename you want to decode: ";
    std::cin >> fileName;

    std::ifstream inFile(fileName, std::ios_base::binary);
    std::ofstream outFile("decoded_"+fileName, std::ios_base::binary | std::ios_base::trunc);

    setCursor({ 2, 13 });
    DrawInterface();

    std::cout << "Файл расшифровывается..." << std::endl;

    while (!inFile.eof())
    {
        inFile.read(buf, 16);
        if (kb >= 128)
        {
            for (int i = 0; i < inFile.gcount(); i++)
            {
                buf[i] = buf[i] ^ generatorLRC();
            }
        }
        kb++;
        outFile.write(buf, inFile.gcount());
    }
    std::cout << "Файл расшифрован!" << std::endl;
    inFile.close(); 
    outFile.close();
    resetCursor();
}

HANDLE MainWindow::getHandleStdOut()
{
    return hStdout;
}

void MainWindow::setExitFlag(bool flag)
{
    isExitFlagSet = flag;
}

char MainWindow::generatorLRC()
{
    int L;
    char Gamma = 0;
    for (int j = 0; j < 10; j++)
    {
        L = LRC2[1] & (LRC2[90] | LRC2[88]);
        Gamma <<= 1;
        Gamma |= L;

        LRC1 >>= 1;
        LRC1[88] = LRC1[88] ^ LRC1[5] ^ LRC1[4] ^ LRC1[2];
        LRC2 >>= 1;
        LRC2[90] = LRC2[90] ^ LRC2[7] ^ LRC2[4] ^ LRC2[0];
        if (LRC1[19] == 0) {
            LRC1 >>= 1;
            LRC1[88] = LRC1[88] ^ LRC1[5] ^ LRC1[4] ^ LRC1[2];
        }
    }
    return Gamma;
}

void MainWindow::CreateElements()
{
    //TEXT_BOX
    winBoxes.insert(std::pair<std::string, WinBox*>("TEXT_BOX_1", new WinBox(COORD(1, 1))));
    winBoxes["TEXT_BOX_1"]->addElementWidthVerticalAlignment(new TextBox(8, 110, "|Info|", LEGEND));
    winBoxes["TEXT_BOX_1"]->addElementWidthVerticalAlignment(new TextBox(7, 110, "|Input|", OUTPUT));
    //BUTTON_BOX
    winBoxes.insert(std::pair<std::string, WinBox*>("BUTTON_BOX_1", new WinBox(COORD(1, 17))));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Encode by Polybius", ENCODE_POLYBIUS));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Decode by Polybius", DECODE_POLYBIUS));

    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Encode by GOST", ENCODE_GOST));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Decode by GOST", DECODE_GOST));

    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Encode lab3", ENCODE_LAB3));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Decode lab3", DECODE_LAB3));
    winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Close", CLOSE));

    //winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Fill array", FILL_ARRAY));
    //winBoxes["BUTTON_BOX_1"]->addElementWidthHorizontalAlignment(new Button(1, "Handle data", HANDLE_DATA));
}

int MainWindow::getAsciiTableIndex(char symbol)
{
    int index = 0;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            if (asciiTableArr[i][j] == symbol) {
                index = i;
            }
        }
    }
    return index;
}

void MainWindow::setCursor(COORD position)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, position);
}

int MainWindow::getKeyIndex(char symbol)
{
    return 0;
}

void MainWindow::resetCursor()
{
    COORD position = { 0,0 }; //позиция x и y
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, position);
}
