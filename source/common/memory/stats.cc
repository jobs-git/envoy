#include "common/memory/stats.h"

#include <cstdint>

#include "common/common/logger.h"

#ifdef TCMALLOC

#include "gperftools/malloc_extension.h"

namespace Envoy {
namespace Memory {

uint64_t Stats::totalCurrentlyAllocated() {
  size_t value = 0;
  MallocExtension::instance()->GetNumericProperty("generic.current_allocated_bytes", &value);
  return value;
}

uint64_t Stats::totalCurrentlyReserved() {
  size_t value = 0;
  MallocExtension::instance()->GetNumericProperty("generic.heap_size", &value);
  return value;
}

uint64_t Stats::totalThreadCacheBytes() {
  size_t value = 0;
  MallocExtension::instance()->GetNumericProperty("tcmalloc.current_total_thread_cache_bytes",
                                                  &value);
  return value;
}

uint64_t Stats::totalPageHeapFree() {
  size_t value = 0;
  MallocExtension::instance()->GetNumericProperty("tcmalloc.pageheap_free_bytes", &value);
  return value;
}

uint64_t Stats::totalPageHeapUnmapped() {
  size_t value = 0;
  MallocExtension::instance()->GetNumericProperty("tcmalloc.pageheap_unmapped_bytes", &value);
  return value;
}

void Stats::dumpStatsToLog() {
  constexpr int buffer_size = 100000;
  auto buffer = std::make_unique<char[]>(buffer_size);
  MallocExtension::instance()->GetStats(buffer.get(), buffer_size);
  ENVOY_LOG_MISC(debug, "TCMalloc stats:\n{}", buffer.get());
}

} // namespace Memory
} // namespace Envoy

#else

namespace Envoy {
namespace Memory {

uint64_t Stats::totalCurrentlyAllocated() { return 0; }
uint64_t Stats::totalThreadCacheBytes() { return 0; }
uint64_t Stats::totalCurrentlyReserved() { return 0; }
uint64_t Stats::totalPageHeapUnmapped() { return 0; }
uint64_t Stats::totalPageHeapFree() { return 0; }
void Stats::dumpStatsToLog() {}

} // namespace Memory
} // namespace Envoy

#endif // #ifdef TCMALLOC
