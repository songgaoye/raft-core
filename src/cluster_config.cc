#include "raft_core/cluster_config.h"

#include <algorithm>

namespace raft_core {

ClusterConfig::ClusterConfig(std::initializer_list<PeerId> peers) {
  for (const PeerId peer : peers) {
    add_peer(peer);
  }
}

bool ClusterConfig::add_peer(PeerId peer) {
  if (contains(peer)) {
    return false;
  }
  peers_.push_back(peer);
  return true;
}

bool ClusterConfig::contains(PeerId peer) const noexcept {
  return std::find(peers_.begin(), peers_.end(), peer) != peers_.end();
}

std::size_t ClusterConfig::size() const noexcept {
  return peers_.size();
}

bool ClusterConfig::empty() const noexcept {
  return peers_.empty();
}

const std::vector<PeerId>& ClusterConfig::peers() const noexcept {
  return peers_;
}

}  // namespace raft_core
