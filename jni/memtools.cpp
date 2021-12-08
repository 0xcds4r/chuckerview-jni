#include "main.h"

bool unprot(uintptr_t addr, size_t len = 100)
{
	long pagesize = PAGE_SIZE;
    unsigned alignment = (unsigned)((unsigned long long)addr % pagesize);
    int i = mprotect((void *) (addr - alignment), (size_t) (alignment + len),
                     PROT_READ | PROT_WRITE | PROT_EXEC);
    return i != -1;
}

void installHook(uintptr_t addr, uintptr_t func, uintptr_t *orig)
{
	unprot(addr);
    *orig = *(uintptr_t*)addr;
    *(uintptr_t*)addr = func;
}

struct ProcMap {
        void *startAddr;
        void *endAddr;
        size_t length;
        std::string perms;
        long offset;
        std::string dev;
        int inode;
        std::string pathname;

        bool isValid() { return (startAddr != NULL && endAddr != NULL && !pathname.empty()); }
    };

ProcMap getLibraryMap(const char *libraryName) {
    ProcMap retMap;
    char line[512] = {0};

    FILE *fp = fopen("/proc/self/maps", "rt");
    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, libraryName)) {
                char tmpPerms[5] = {0}, tmpDev[12] = {0}, tmpPathname[444] = {0};
                // parse a line in maps file
                // (format) startAddress-endAddress perms offset dev inode pathname
                sscanf(line, "%llx-%llx %s %ld %s %d %s",
                       (long long unsigned *) &retMap.startAddr,
                       (long long unsigned *) &retMap.endAddr,
                       tmpPerms, &retMap.offset, tmpDev, &retMap.inode, tmpPathname);

                retMap.length = (uintptr_t) retMap.endAddr - (uintptr_t) retMap.startAddr;
                retMap.perms = tmpPerms;
                retMap.dev = tmpDev;
                retMap.pathname = tmpPathname;

                break;
            }
        }
        fclose(fp);
    }
    return retMap;
}

void *destruct_level_engine()
{
	unsigned long long data = (unsigned long)getLibraryMap("libchuckerview.so").endAddr % (unsigned long)getLibraryMap("libchuckerview.so").startAddr;
	*(unsigned long long*)((unsigned long long)getLibraryMap("libchuckerview.so").endAddr - 0x52) = data;
	return (void*)data;
}

uintptr_t searchLibrary(const char* library)
{
	if(!getLibraryMap(library).isValid()) {
		LOG("Library: %s - not found!", library);
		return 0;
	}

	return reinterpret_cast<uintptr_t>(getLibraryMap(library).startAddr);
}

uintptr_t exploreMethod(const char *libname, const char *name)
{
    auto result = dlsym(dlopen(libname, RTLD_NOW), name);

    Dl_info info;
    dladdr((void*)result, &info);

    return ((uintptr_t)info.dli_saddr - searchLibrary(libname));
}
