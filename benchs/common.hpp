// Cf Carruth cppcon 2015.  https://www.youtube.com/watch?v=nXaxk27zwlk
static void escape(void* p) { asm volatile("" : : "g"(p) : "memory"); }
static void clobber() { asm volatile("" : : : "memory"); }



