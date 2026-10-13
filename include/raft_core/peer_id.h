#ifndef RAFT_CORE_PEER_ID_H_
#define RAFT_CORE_PEER_ID_H_

#include <cstdint>

namespace raft_core {

class PeerId {
 public:
  explicit PeerId(std::uint32_t value) noexcept : value_(value) {
  }

  std::uint32_t value() const noexcept {
    return value_;
  }

  friend bool operator==(PeerId lhs, PeerId rhs) noexcept {
    return lhs.value_ == rhs.value_;
  }

  friend bool operator!=(PeerId lhs, PeerId rhs) noexcept {
    return !(lhs == rhs);
  }

 private:
  std::uint32_t value_;
};

}  // namespace raft_core

#endif  // RAFT_CORE_PEER_ID_H_
