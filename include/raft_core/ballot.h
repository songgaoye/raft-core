#ifndef RAFT_CORE_BALLOT_H_
#define RAFT_CORE_BALLOT_H_

#include "raft_core/cluster_config.h"

#include <optional>
#include <vector>

namespace raft_core {

// Tracks acknowledgements for one log entry. When old_config is present, both
// the current and old configurations must reach a majority.
class Ballot {
 public:
  explicit Ballot(const ClusterConfig& current_config,
                  const ClusterConfig* old_config = nullptr);

  // Duplicate or unknown acknowledgements are ignored.
  void grant(PeerId peer);
  bool granted() const noexcept;

 private:
  static std::size_t majority_for(const ClusterConfig& config) noexcept;
  static bool erase_pending(std::vector<PeerId>* pending, PeerId peer) noexcept;

  std::vector<PeerId> current_pending_;
  std::optional<std::vector<PeerId>> old_pending_;
  std::size_t current_required_;
  std::optional<std::size_t> old_required_;
  std::size_t current_granted_ = 0;
  std::optional<std::size_t> old_granted_;
};

}  // namespace raft_core

#endif  // RAFT_CORE_BALLOT_H_
