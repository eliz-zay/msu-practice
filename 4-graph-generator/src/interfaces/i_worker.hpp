#pragma once

namespace uni_course_cpp {
class IWorker {
 public:
  virtual ~IWorker(){};

  virtual void start() = 0;
  virtual void stop() = 0;
};
}  // namespace uni_course_cpp
