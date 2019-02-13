//===- Counter.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ANALYSIS_COUNTER_H
#define ONNC_ANALYSIS_COUNTER_H
#include <onnc/ADT/StringRef.h>
#include <onnc/JSON/Group.h>

namespace onnc {

/** \class Counter
 *  \brief Counter provides an interface for statistical counting.
 *
 *  A counter object is an abstraction of json::Group object. It defines a
 *  format of json::Group to represent a counter when doing statistics.
 *  A counter has four entries:
 *  - "name" : [string],
 *  - "type" : [a magic integer],
 *  - "description" : [string],
 *  - "value" : [integer]
 *  Name should be a unique identity of a counter. Type is a fixed magic number.
 *  description is a string, and value keeps the value of a counter.
 *
 *  Users can use @ref Counter::Create function to create an empty counter. Or
 *  use @ref Counter::Get to get the counter object from a well-settle group.
 *
 *  In case users want to claim a Counter object and then posphone assignment of
 *  value, we also provide default constructor and assign operator for this case.
 *  @ref Counter::isValid return true if a Counter object already has well-settled
 *  group.
 */
class Counter final
{
public:
  using value_type = int;

  static constexpr value_type defaultValue  = 0;
  static constexpr char       defaultDesc[] = "none";

public:
  friend bool isValid(const Counter&);

  /// Default constructor. This create an empty group object without any value.
  /// In this case, the counter object is invalid and users can not increase/
  /// decrease its value.
  Counter() = delete;

  /// Constructor. Users can get or create counter be give a name. If there
  /// is already a counter with the same name, just get the existing counter.
  /// otherwise create a new counter with that name. Users can also provide
  /// a default value and description for the counter.
  Counter(StringRef pName, value_type pValue = defaultValue, StringRef pDesc = defaultDesc);

  /// Conversion constructor. This create a counter with an associated group object.
  /// The @ref Counter::Create() method can create Counter objects by invoking this
  /// constructor, and the created counter objects are all valid.
  explicit Counter(json::Group group);

  /// Copy constructor. Copy the value of the counter from the others. Since
  /// a Counter object is just an abstract interface of a json::Group object,
  /// this copy shall not create new json::Group object; the copy operator
  /// just create a new representation of the original json::Group object.
  Counter(const Counter& pOther) = default;
  Counter(Counter&& pOther) = default;

public:
  /// Assignment. Since a Counter object is just an abstract interface of a
  /// json::Group object, this assignment shall not create new json::Group object;
  /// the assignment operator just create a new representation of the original
  /// json::Group object.
  Counter& operator=(const Counter& pOther) = default;
  Counter& operator=(Counter&& pOther) = default;

  ~Counter() = default;

  /// update counter value by given @ref pNumber.
  Counter& operator=(value_type pNumber);

  /// The identity of the counter.
  StringRef name() const;

  /// The description of the counter.
  StringRef desc() const;

  /// Set description.
  Counter& desc(StringRef pDesc);

  /// Casting operator. Cast to the value.
  operator value_type() const;

  Counter& operator++();

  /// The postfix form of ++.
  Counter& operator++(int);

  Counter& operator--();

  /// The postfix form of --.
  Counter& operator--(int);

  Counter& operator+=(value_type number);

  Counter& operator-=(value_type number);

private:
  json::Group m_Group;
};

/// @retval true pGroup is a counter group.
bool isCounter(const json::Group& pGroup);

/// If a counter wasn't given a group, it isn't active.
/// A Counter object created by default constructor is invalid.
bool isValid(const Counter& counter);

std::ostream& operator<<(std::ostream& stream, const Counter& counter);

} // namespace of onnc

#endif
