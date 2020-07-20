#include <stdio.h>
#include <Windows.h>
#include <Imagehlp.H>
#include<iostream>
#pragma comment(lib,"Imagehlp.lib")

HANDLE OpenPeByFileName(LPTSTR FileName)
{
    LPTSTR PortableExecutableFileName = FileName;//��ȡPE�ļ�����
    HANDLE hFile, hMapFile, hMapAddress = NULL;
    DWORD dwFileSize = 0;

    hFile = CreateFile(PortableExecutableFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//��ȡ������
    dwFileSize = GetFileSize(hFile, NULL);
    std::cout<<"�ļ���С��                                       " << dwFileSize<<std::endl;
    hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);//�����ļ�ӳ��
    hMapAddress = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, dwFileSize);//�ڵ��ý��̵ĵ�ַ�ռ�ӳ��һ���ļ���ͼ������һ��ӳ����ͼ����ʼ��ַ
                                                                           //��B���̵��ļ�ӳ���ŵ�A�У�������ͨ��
    if (hMapAddress != NULL)
    {
        CloseHandle(hFile);
        CloseHandle(hMapFile);
        return hMapAddress;
    }
}
BOOL IsPEFile(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    if (ImageBase == NULL) { return FALSE; }
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    if (IMAGE_DOS_SIGNATURE != pDosHead->e_magic) { return FALSE; }
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    if (IMAGE_NT_SIGNATURE != pNtHead->Signature) { return FALSE; }
    return TRUE;
}
PIMAGE_NT_HEADERS GetNtHead(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    return pNtHead;
}

void DisplayDOSHeadInfo(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    std::cout << "-------------------------DOSHead-------------------------" << std::endl;
    printf("DOSǩ��(MZSignature)��                           %x\n", pDosHead->e_magic);
    printf("�ļ����ҳ���ֽ���(UsedBytesInTheLastPage)��     %x\n", pDosHead->e_cblp);
    printf("�ļ�ҳ��(FileSizeInPages)��                      %x\n", pDosHead->e_cp);
    printf("�ض���Ԫ�ظ���(NumberOfRelocationItems)��        %x\n", pDosHead->e_crlc);
    printf("ͷ���ߴ�(HeaderSizeInParagraphs)��               %x\n", pDosHead->e_cparhdr);
    printf("�������С���Ӷ�(MinimumExtraParagraphs)��       %x\n", pDosHead->e_minalloc);
    printf("�������󸽼Ӷ�(MaximumExtraParagraphs)��       %x\n", pDosHead->e_maxalloc);
    printf("��ʼ��SSֵ(InitialRelativeSS)��                  %x\n", pDosHead->e_ss);
    printf("��ʼ��SPֵ(InitialSP)��                          %x\n", pDosHead->e_sp);
    printf("У���(Checksum)��                               %x\n", pDosHead->e_csum);
    printf("��ʼ��IPֵ(InitialIP)��                          %x\n", pDosHead->e_ip);
    printf("��ʼ��CSֵ(InitialRelativeCS)��                  %x\n", pDosHead->e_cs);
    printf("�ط�����ļ���ַ(AddressOfRelocationTable)��     %x\n", pDosHead->e_lfarlc);
    printf("���Ǻ�(OverlayNumber)��                          %x\n", pDosHead->e_ovno);
    printf("������[4](Reserved)��\n");
    printf("Reserved[0]��                                    %x\n", pDosHead->e_res[0]);
    printf("Reserved[1]��                                    %x\n", pDosHead->e_res[1]);
    printf("Reserved[2]��                                    %x\n", pDosHead->e_res[2]);
    printf("Reserved[3]��                                    %x\n", pDosHead->e_res[3]);
    printf("OEM��ʶ��(OEMid)��                               %x\n", pDosHead->e_oemid);
    printf("OEM��Ϣ(OEMinfo)��                               %x\n", pDosHead->e_oeminfo);
    printf("������[10](Reserved2)��\n");
    printf("Reserved2[0]��                                   %x\n", pDosHead->e_res2[0]);
    printf("Reserved2[1]��                                   %x\n", pDosHead->e_res2[1]);
    printf("Reserved2[2]��                                   %x\n", pDosHead->e_res2[2]);
    printf("Reserved2[3]��                                   %x\n", pDosHead->e_res2[3]);
    printf("Reserved2[4]��                                   %x\n", pDosHead->e_res2[4]);
    printf("Reserved2[5]��                                   %x\n", pDosHead->e_res2[5]);
    printf("Reserved2[6]��                                   %x\n", pDosHead->e_res2[6]);
    printf("Reserved2[7]��                                   %x\n", pDosHead->e_res2[7]);
    printf("Reserved2[8]��                                   %x\n", pDosHead->e_res2[8]);
    printf("Reserved2[9]��                                   %x\n", pDosHead->e_res2[9]);
    printf("ָʾNTͷ��ƫ��(AddressOfNewExeHeader)��          %x\n", pDosHead->e_lfanew);
}


void DisplayFileHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pNtHead = GetNtHead(ImageBase);
    std::cout << "-------------------------FileHeader-------------------------" << std::endl;
    printf("ǩ��(Signatur):                                  %x\n", pNtHead->Signature);
    printf("����ƽ̨(Machine):                               %x\n", pNtHead->FileHeader.Machine);
    printf("������Ŀ(NumberOfSections):                      %x\n", pNtHead->FileHeader.NumberOfSections);
    printf("����ʱ��(TimeDateStamp):                         %x\n", pNtHead->FileHeader.TimeDateStamp);
    printf("COFF�ļ����ű�ƫ��(PointerToSymbolTable):        %x\n", pNtHead->FileHeader.PointerToSymbolTable);
    printf("���ű������(NumberOfSymbols):                   %x\n", pNtHead->FileHeader.NumberOfSymbols);
    printf("��ѡͷ��С(SizeOfOptionalHeader):                %x\n", pNtHead->FileHeader.SizeOfOptionalHeader);
    printf("�ļ�����(Characteristics):                       %x\n", pNtHead->FileHeader.Characteristics);
}

void DisplayOptionalHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pNtHead = GetNtHead(ImageBase);
    std::cout << "-------------------------OptionalHeader-------------------------" << std::endl;
    printf("ħ��(Magic)��                                    %x\n", pNtHead->OptionalHeader.Magic);
    printf("�����������汾��(MajorLinkerVersion)��           %x\n", pNtHead->OptionalHeader.MajorLinkerVersion);
    printf("�������Ĵΰ汾��(MinorLinkerVersion)��           %x\n", pNtHead->OptionalHeader.MinorLinkerVersion);
    printf("����ڴ�С(SizeOfCode)��                         %x\n", pNtHead->OptionalHeader.SizeOfCode);
    printf("�ѳ�ʼ������С(SizeOfInitializedData)��          %x\n", pNtHead->OptionalHeader.SizeOfInitializedData);
    printf("δ��ʼ������С(SizeOfUninitializedData)��        %x\n", pNtHead->OptionalHeader.SizeOfUninitializedData);
    printf("����ִ�д�����ʼ��ַ(AddressOfEntryPoint)��      %x\n", pNtHead->OptionalHeader.AddressOfEntryPoint);
    printf("�����ַ(BaseOfCode)��                           %x\n", pNtHead->OptionalHeader.BaseOfCode);
    printf("���ݻ�ַ(BaseOfData)��                           %x\n", pNtHead->OptionalHeader.BaseOfData);
    printf("�����׵�ַ(ImageBase)��                          %x\n", pNtHead->OptionalHeader.ImageBase);
    printf("�ڶ����ڴ��е���С��λ(SectionAlignment)��       %x\n", pNtHead->OptionalHeader.SectionAlignment);
    printf("�ڶ��ڴ����ļ��е���С��λ(FileAlignment)��      %x\n", pNtHead->OptionalHeader.FileAlignment);
    printf("��ϵͳ�����汾��(MajorOperatingSystemVersion)��  %x\n", pNtHead->OptionalHeader.MajorOperatingSystemVersion);
    printf("��ϵͳ�Ĵΰ汾��(MinorOperatingSystemVersion)��  %x\n", pNtHead->OptionalHeader.MinorOperatingSystemVersion);
    printf("��������汾��(MajorImageVersion)��              %x\n", pNtHead->OptionalHeader.MajorImageVersion);
    printf("����Ĵΰ汾��(MinorImageVersion)��              %x\n", pNtHead->OptionalHeader.MinorImageVersion);
    printf("��ϵͳ�����汾��(MajorSubsystemVersion)��        %x\n", pNtHead->OptionalHeader.MajorSubsystemVersion);
    printf("��ϵͳ�Ĵΰ汾��(MinorSubsystemVersion)��        %x\n", pNtHead->OptionalHeader.MinorSubsystemVersion);
    printf("�����ֶ�(Win32VersionValue)��                    %x\n", pNtHead->OptionalHeader.Win32VersionValue);
    printf("���񱻼��ؽ��ڴ�ʱ�Ĵ�С(SizeOfImage)��          %x\n", pNtHead->OptionalHeader.SizeOfImage);
    printf("����ͷ���ܴ�С(SizeOfHeaders)��                  %x\n", pNtHead->OptionalHeader.SizeOfHeaders);
    printf("�����ļ���У���(CheckSum)��                     %x\n", pNtHead->OptionalHeader.CheckSum);
    printf("���д˾����������ϵͳ(Subsystem)��              %x\n", pNtHead->OptionalHeader.Subsystem);
    printf("DLL��ʶ(DllCharacteristics)��                    %x\n", pNtHead->OptionalHeader.DllCharacteristics);
    printf("���ջ��С(SizeOfStackReserve)��                 %x\n", pNtHead->OptionalHeader.SizeOfStackReserve);
    printf("��ʼ�ύ�Ķ�ջ��С(SizeOfStackCommit)��          %x\n", pNtHead->OptionalHeader.SizeOfStackCommit);
    printf("���Ѵ�С(SizeOfHeapReserve)��                  %x\n", pNtHead->OptionalHeader.SizeOfHeapReserve);
    printf("��ʼ�ύ�ľֲ��ѿռ��С(SizeOfHeapCommit)��     %x\n", pNtHead->OptionalHeader.SizeOfHeapCommit);
    printf("�����ֶ�(LoaderFlags)��                          %x\n", pNtHead->OptionalHeader.LoaderFlags);
    printf("DataDirectory���������(NumberOfRvaAndSizes)��   %x\n", pNtHead->OptionalHeader.NumberOfRvaAndSizes);
}
//IMAGE_DATA_DIRECTORY��չʾд���о�ûɶ��������
//void DisplayDataDirectoryInfo(HANDLE ImageBase)
//{
//    PIMAGE_NT_HEADERS pNtHead = NULL;
//    pNtHead = GetNtHead(ImageBase);
//    printf("���ݵ���������ַ(VirtualAddress):                               %x\n", pNtHead->OptionalHeader.DataDirectory->VirtualAddress);
//    printf("���ݿ�Ĵ�С(Size):                      %x\n", pNtHead->OptionalHeader.DataDirectory->Size);
//
//}

void DisplaySectionHeaderInfo(HANDLE ImageBase)
{
    PIMAGE_NT_HEADERS pNtHead = NULL;
    PIMAGE_FILE_HEADER pFileHead = NULL;
    PIMAGE_SECTION_HEADER pSection = NULL;
    DWORD NumberOfSectinsCount = 0;
    pNtHead = GetNtHead(ImageBase);
    pSection = IMAGE_FIRST_SECTION(pNtHead);
    pFileHead = &pNtHead->FileHeader;

    NumberOfSectinsCount = pFileHead->NumberOfSections;        // �����������
    DWORD* difA = NULL;   // �����ַ��ͷ
    DWORD* difS = NULL;   // ���ƫ��(���ڱ���)
    difA = (DWORD*)malloc(NumberOfSectinsCount * sizeof(DWORD));
    difS = (DWORD*)malloc(NumberOfSectinsCount * sizeof(DWORD));
    std::cout << "-------------------------SectionHeader-------------------------" << std::endl;
    printf("�������� ���ƫ��\t�����С\tRaw����ָ��\tRaw���ݴ�С\t��������\n");
    for (int temp = 0; temp < NumberOfSectinsCount; temp++, pSection++)
    {
        printf("%s\t 0x%.8X \t 0x%.8X \t 0x%.8X \t 0x%.8X \t 0x%.8X\n",
            pSection->Name, pSection->VirtualAddress, pSection->Misc.VirtualSize,
            pSection->PointerToRawData, pSection->SizeOfRawData, pSection->Characteristics);
        difA[temp] = pSection->VirtualAddress;
        difS[temp] = pSection->VirtualAddress - pSection->PointerToRawData;
    }
}


//VOID DisplayExportTable(HANDLE ImageBase)
//{
//    IMAGE_IMPORT_DESCRIPTOR* pExport = 0;
//    PIMAGE_DOS_HEADER pDosHead = NULL;
//    PIMAGE_NT_HEADERS pNtHead = NULL;
//    IMAGE_SECTION_HEADER* pSectionHeader = 0;
//    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
//    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
//    if (pNtHead->Signature != 0x00004550) { return; }        // ��ЧPE�ļ�
//    //if (pNtHead->OptionalHeader.Magic != 0x20b){return;}  // ����64λPE
//
////��λ����һ���ڵĵ�ַ
//    pSectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)pNtHead + sizeof(IMAGE_NT_HEADERS));
//    //ntHeader->FileHeader.NumberOfSectionsΪ�ڵ��������˴�ѭ�����ҵ������Ľ�
//    for (int i = 0; i < pNtHead->FileHeader.NumberOfSections; i++)
//    {
//        //IMAGE_DIRECTORY_ENTRY_IMPORT�������
//        if (pSectionHeader->VirtualAddress == pNtHead->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress)
//        {
//            break;
//        }
//        //û�ҵ�����ô����SizeOf(IMAGE_SECTION_HEADER)�ֽ�����ָ����һ����
//        pSectionHeader++;
//    }
//    //��������
//    pExport = (IMAGE_IMPORT_DESCRIPTOR*)((DWORD)ImageBase + pSectionHeader->PointerToRawData);
//    //���ص�DLL�����ƣ�������RVA��ַ����Ҫת�����ļ�ƫ�Ƶ�ַ����Ϊ���ǲ���ͨ��PE���������أ�����ӳ�䵽�ڴ�
//    while (pExport->Name != 0)
//    {
//        DWORD offset = (DWORD)ImageBase + pExport->Name - (pSectionHeader->VirtualAddress - pSectionHeader->PointerToRawData);
//        char* s = (char*)offset;
//        std::cout << s << std::endl;
//        //������ȡ
//        pExport++;
//    }

//
//}

void DisplayImportTable(HANDLE ImageBase)
{
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    PIMAGE_IMPORT_DESCRIPTOR pInput = NULL;
    PIMAGE_THUNK_DATA _pThunk = NULL;
    DWORD dwThunk = NULL;
    DWORD dwOriginalFirstThunk = NULL;
    USHORT Hint;
    int iSystemBits = 0;

    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = GetNtHead(ImageBase);
    //�жϰ汾��32λ����64λ
    if (pNtHead->FileHeader.Machine== IMAGE_FILE_MACHINE_I386)
    {
        iSystemBits = 32;
    }
    if (pNtHead->FileHeader.Machine == (IMAGE_FILE_MACHINE_IA64 || IMAGE_FILE_MACHINE_AMD64))
    {
        iSystemBits = 64;
    }
    if (pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress == 0) { return; }  // ��ȡ�����RVA
    pInput = (PIMAGE_IMPORT_DESCRIPTOR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, pNtHead->OptionalHeader.DataDirectory[1].VirtualAddress, NULL);
    std::cout << "-------------------------ImportTable-------------------------" << std::endl;
    for (; pInput->Name != NULL;)
    {
        char* szFunctionModule = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)pInput->Name, NULL);  // ������ģ������
        dwThunk = pInput->FirstThunk;
        dwOriginalFirstThunk= pInput->OriginalFirstThunk;
        _pThunk = (PIMAGE_THUNK_DATA)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)pInput->FirstThunk, NULL);
        
        for (; _pThunk->u1.AddressOfData != NULL;)
        {
            
            char* szFunction = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)(((PIMAGE_IMPORT_BY_NAME)_pThunk->u1.AddressOfData)->Name), 0);
            if (szFunction != NULL)
            {
                memcpy(&Hint, szFunction - 2, 2);
            }
            else
            {
                Hint = -1;
            }
            if (Hint== NULL || dwThunk == NULL || dwOriginalFirstThunk == NULL || szFunctionModule == NULL || szFunction == NULL)
            {

            }
            else
            {
                std::cout << "��ţ�" << Hint << std::endl;
                std::cout << "Thunk RVA��" << std::hex << dwThunk << std::endl;
                std::cout << "OriginalFirstThunk��" << std::hex << dwOriginalFirstThunk << std::endl;
                std::cout << "DLL���ƣ�" << szFunctionModule << std::endl;
                std::cout << "API���ƣ�" << szFunction << std::endl;
                std::cout << "--------------------------------------------------" << std::endl;
            }

            //32λ +4 64λ+8
            if (iSystemBits == 32)
            {
                dwThunk += 4;
            }
            else if (iSystemBits == 64)
            {
                dwThunk += 8;
            }
            _pThunk++;
        }
        pInput++;
    }
}
VOID DisplayExportTable(HANDLE ImageBase)
{

    PIMAGE_EXPORT_DIRECTORY pExport;
    PIMAGE_DOS_HEADER pDosHead = NULL;
    PIMAGE_NT_HEADERS pNtHead = NULL;
    pDosHead = (PIMAGE_DOS_HEADER)ImageBase;
    pNtHead = (PIMAGE_NT_HEADERS)((DWORD)pDosHead + pDosHead->e_lfanew);
    if (pNtHead->Signature != 0x00004550) { return; }        // ��ЧPE�ļ�

    pExport = (PIMAGE_EXPORT_DIRECTORY)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, pNtHead->OptionalHeader.DataDirectory[0].VirtualAddress, NULL);
    DWORD NumberOfNames = pExport->NumberOfNames;
    ULONGLONG** ppdwNames = (ULONGLONG**)pExport->AddressOfNames;
    ppdwNames = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)ppdwNames, NULL);
    ULONGLONG** ppdwAddr = (ULONGLONG**)pExport->AddressOfFunctions;
    ppdwAddr = (PULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (DWORD)ppdwAddr, NULL);
    ULONGLONG* ppdwOrdin = (ULONGLONG*)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (DWORD)pExport->AddressOfNameOrdinals, NULL);
    std::cout << "-------------------------ExportTable-------------------------" << std::endl;
    char* szFunction = (PSTR)ImageRvaToVa((PIMAGE_NT_HEADERS)pNtHead, pDosHead, (ULONG)*ppdwNames, NULL);
    std::cout << "����ֵ��" << pExport->Characteristics << std::endl;
    std::cout << "ʱ�����ڱ�־��" << std::hex << pExport->TimeDateStamp << std::endl;
    //std::cout  <<"��汾��"<< pExport->MajorVersion << std::endl;
    //std::cout << "С�汾��" << pExport->MinorVersion << std::endl;
    std::cout << "�汾�ţ�" << std::hex << pExport->MajorVersion << "." << std::hex << pExport->MinorVersion << std::endl;
    std::cout << "���ƣ�" << pExport->Name << std::endl;
    std::cout << "��ַ��" << pExport->Base << std::endl;
    std::cout << "����������" << pExport->NumberOfFunctions << std::endl;
    std::cout << "���Ƹ�����" << pExport->NumberOfNames << std::endl;
    std::cout << "������ַ��" << pExport->AddressOfFunctions << std::endl;
    std::cout << "���Ƶ�ַ��" << pExport->AddressOfNames << std::endl;
    std::cout << "����������ַ��" << pExport->AddressOfNameOrdinals << std::endl;
    for (DWORD i = 0; i < NumberOfNames; i++)
    {
        std::cout << "��ţ�" << i << std::endl;
        std::cout << "RVA" << *ppdwAddr << std::endl;
        std::cout << "���ƣ�" << szFunction << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        szFunction = szFunction + strlen(szFunction) + 1;
        ppdwAddr++;
    }
}
int main()
{
    HANDLE lpMapAddress = NULL;
    wchar_t * wcFileName = L"C:\\Users\\ascotbe\\Desktop\\UplayInstaller.exe";//���ļ���λ��
    lpMapAddress = OpenPeByFileName(wcFileName);
    DisplayDOSHeadInfo(lpMapAddress);
    IsPEFile(lpMapAddress);
    //DisplayDataDirectoryInfo(lpMapAddress);
    DisplayFileHeaderInfo(lpMapAddress);
    DisplayOptionalHeaderInfo(lpMapAddress);
    DisplaySectionHeaderInfo(lpMapAddress);
    DisplayImportTable(lpMapAddress);
    DisplayExportTable(lpMapAddress);//DLL��ʱ���ٵ����������
    return 0;
}
