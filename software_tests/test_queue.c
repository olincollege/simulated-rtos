#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#include "../src/queue.h"

// NOLINTBEGIN(*-magic-numbers)

// Test that enqueue queues a node on an existing queue
Test(enqueue, non_integer_mean) {
  const int array[] = {1, 2};
  size_t array_len = 2;
  cr_assert_float_eq(mean(array, array_len), 1.5, 0.0001,
                     "Expected ~1.5, but got %f", mean(array, array_len));
}

// Test that enqueue queues a node on an empty queue

Test(pop, non_integer_mean) {
  const int array[] = {1, 2};
  size_t array_len = 2;
  cr_assert_float_eq(mean(array, array_len), 1.5, 0.0001,
                     "Expected ~1.5, but got %f", mean(array, array_len));
}

// NOLINTEND(*-magic-numbers)
