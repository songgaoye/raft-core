#include "raft_core/simulated_network.h"

#include <cassert>
#include <stdexcept>
#include <vector>

namespace raft_core {
namespace {

const PeerId kNodeA(1);
const PeerId kNodeB(2);
const PeerId kNodeC(3);

SimulatedMessage message(PeerId from, PeerId to, std::uint64_t tag) {
  return SimulatedMessage{from, to, tag};
}

void assert_tags(const std::vector<SimulatedMessage>& messages,
                 std::initializer_list<std::uint64_t> expected) {
  assert(messages.size() == expected.size());
  std::size_t index = 0;
  for (const std::uint64_t tag : expected) {
    assert(messages[index].tag == tag);
    ++index;
  }
}

void test_delays_and_virtual_time() {
  SimulatedNetwork network;
  network.send(message(kNodeA, kNodeB, 1), 10);
  network.send(message(kNodeA, kNodeC, 2), 5);

  assert(network.now() == 0);
  assert(network.pending_count() == 2);
  assert_tags(network.advance_to(4), {});
  assert(network.now() == 4);
  assert_tags(network.advance_to(5), {2});
  assert_tags(network.advance_to(10), {1});
  assert(network.pending_count() == 0);
}

void test_equal_deadlines_preserve_send_order() {
  SimulatedNetwork network;
  network.send(message(kNodeA, kNodeB, 10), 7);
  network.send(message(kNodeB, kNodeA, 11), 7);
  network.send(message(kNodeC, kNodeA, 12), 7);

  assert_tags(network.advance_to(7), {10, 11, 12});
}

void test_directed_partition_drops_only_new_messages() {
  SimulatedNetwork network;
  network.send(message(kNodeA, kNodeB, 20), 5);
  network.block_link(kNodeA, kNodeB);
  network.send(message(kNodeA, kNodeB, 21), 1);
  network.send(message(kNodeB, kNodeA, 22), 1);

  assert(network.pending_count() == 2);
  assert_tags(network.advance_to(1), {22});
  assert_tags(network.advance_to(5), {20});

  network.unblock_link(kNodeA, kNodeB);
  network.send(message(kNodeA, kNodeB, 23), 0);
  assert_tags(network.advance_to(5), {23});
}

void test_time_never_moves_backwards() {
  SimulatedNetwork network;
  network.advance_to(10);

  bool threw = false;
  try {
    (void)network.advance_to(9);
  } catch (const std::invalid_argument&) {
    threw = true;
  }
  assert(threw);
  assert(network.now() == 10);
}

}  // namespace
}  // namespace raft_core

int main() {
  using namespace raft_core;

  test_delays_and_virtual_time();
  test_equal_deadlines_preserve_send_order();
  test_directed_partition_drops_only_new_messages();
  test_time_never_moves_backwards();
}
