#include "raft_core/ballot.h"

#include <algorithm>

namespace raft_core {

Ballot::Ballot(const ClusterConfig& current_config, const ClusterConfig* old_config)
    : current_pending_(current_config.peers()), current_required_(majority_for(current_config)) {
  if (old_config != nullptr) {
    old_pending_ = old_config->peers();
    old_required_ = majority_for(*old_config);
    old_granted_ = 0;
  }
}

void Ballot::grant(PeerId peer) {
  if (erase_pending(&current_pending_, peer)) {
    ++current_granted_;
  }

  if (old_pending_ != std::nullopt && erase_pending(&*old_pending_, peer)) {
    ++*old_granted_;
  }
}

bool Ballot::granted() const noexcept {
  if (current_granted_ < current_required_) {
    return false;
  }

  if (old_required_ == std::nullopt) {
    return true;
  }
  return *old_granted_ >= *old_required_;
}

std::size_t Ballot::majority_for(const ClusterConfig& config) noexcept {
  return config.size() / 2 + 1;
}

bool Ballot::erase_pending(std::vector<PeerId>* pending, PeerId peer) noexcept {
  const auto found = std::find(pending->begin(), pending->end(), peer);
  if (found == pending->end()) {
    return false;
  }
  pending->erase(found);
  return true;
}

}  // namespace raft_core
