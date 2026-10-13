#ifndef RAFT_CORE_SIMULATED_NETWORK_H_
#define RAFT_CORE_SIMULATED_NETWORK_H_

#include "raft_core/peer_id.h"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace raft_core {

using Tick = std::uint64_t;

// A protocol-neutral envelope. Raft request/response payloads will replace tag
// in a later stage; the simulator must not own Raft protocol state.
struct SimulatedMessage {
  PeerId from;
  PeerId to;
  std::uint64_t tag;
};

// A deterministic, single-threaded network model. A blocked link drops a
// message when send() is called. Messages already accepted remain in flight.
class SimulatedNetwork {
 public:
  Tick now() const noexcept;
  std::size_t pending_count() const noexcept;

  void block_link(PeerId from, PeerId to);
  void unblock_link(PeerId from, PeerId to);

  // Schedules a message at now() + delay. Equal deadlines are delivered in
  // send order. delay is deliberately virtual and has no wall-clock meaning.
  void send(SimulatedMessage message, Tick delay);

  // Advances virtual time and moves every due message into the returned batch.
  // Throws std::invalid_argument when target is before now().
  std::vector<SimulatedMessage> advance_to(Tick target);

 private:
  struct Link {
    PeerId from;
    PeerId to;
  };

  struct QueuedMessage {
    SimulatedMessage message;
    Tick deadline;
    std::uint64_t sequence;
  };

  bool link_is_blocked(PeerId from, PeerId to) const noexcept;

  Tick now_ = 0;
  std::uint64_t next_sequence_ = 0;
  std::vector<Link> blocked_links_;
  std::vector<QueuedMessage> pending_;
};

}  // namespace raft_core

#endif  // RAFT_CORE_SIMULATED_NETWORK_H_
