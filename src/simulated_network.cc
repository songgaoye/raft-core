#include "raft_core/simulated_network.h"

#include <stdexcept>

namespace raft_core {

Tick SimulatedNetwork::now() const noexcept {
  return now_;
}

std::size_t SimulatedNetwork::pending_count() const noexcept {
  return pending_.size();
}

void SimulatedNetwork::block_link(PeerId from, PeerId to) {
  // TODO: make this idempotent. Links are directed: blocking A -> B must not
  // affect B -> A.
  (void)from;
  (void)to;
}

void SimulatedNetwork::unblock_link(PeerId from, PeerId to) {
  // TODO: make this idempotent.
  (void)from;
  (void)to;
}

void SimulatedNetwork::send(SimulatedMessage message, Tick delay) {
  // TODO: discard messages on a blocked directed link. Otherwise queue the
  // message with deadline now_ + delay and its strictly increasing sequence.
  (void)message;
  (void)delay;
  (void)next_sequence_;
}

std::vector<SimulatedMessage> SimulatedNetwork::advance_to(Tick target) {
  if (target < now_) {
    throw std::invalid_argument("virtual time cannot go backwards");
  }

  // TODO: set now_, return and remove every message with deadline <= now_,
  // sorted by (deadline, sequence).
  now_ = target;
  return {};
}

bool SimulatedNetwork::link_is_blocked(PeerId from, PeerId to) const noexcept {
  // TODO: search blocked_links_.
  (void)from;
  (void)to;
  return false;
}

}  // namespace raft_core
