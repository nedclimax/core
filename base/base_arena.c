#include "base.h"

Memory_Arena make_arena_reserve(Memory_Allocator *a, u64 reserve_size) {
	Memory_Arena result = {0};
	result.allocator = a;
	result.capacity = reserve_size;
	result.memory = a->reserve(a->context, reserve_size);
	return result;
}

Memory_Arena make_arena(Memory_Allocator *a) {
	return make_arena_reserve(a, ARENA_DEFAULT_RESERVE_SIZE);
}

void memory_arena_release(Memory_Arena *arena) {
	Memory_Allocator *a = arena->allocator;
	a->release(a->context, arena->memory, arena->capacity);
}

void *memory_arena_push(Memory_Arena *arena, u64 size) {
	void *result = NULL;
	if (arena->position + size <= arena->capacity) {
		result = arena->memory + arena->position;
		arena->position += size;

		u64 p = arena->position;
		u64 commit_p = arena->commit_position;
		if (p > commit_p) {
			u64 p_aligned = align_up_power_of_two(p, ARENA_COMMIT_BLOCK_SIZE - 1);
			u64 next_commit_p = min(p_aligned, arena->capacity);
			u64 commit_size = next_commit_p - commit_p;
			Memory_Allocator *a = arena->allocator;
			a->commit(a->context, arena->memory + commit_p, commit_size);
			arena->commit_position = next_commit_p;
		}
	}
	return result;
}

void memory_arena_pop_to(Memory_Arena *arena, u64 position) {
	if (position < arena->position) {
		arena->position = position;

		u64 p = arena->position;
		u64 p_aligned = align_up_power_of_two(p, ARENA_COMMIT_BLOCK_SIZE - 1);
		u64 next_commit_p = min(p_aligned, arena->capacity);

		u64 commit_p = arena->commit_position;
		if (next_commit_p < commit_p) {
			u64 decommit_size = commit_p - next_commit_p;
			Memory_Allocator *a = arena->allocator;
			a->decommit(a->context, arena->memory + next_commit_p, decommit_size);
			arena->commit_position = next_commit_p;
		}
	}
}

Temp_Arena memory_arena_begin_temp(Memory_Arena *arena) {
	Temp_Arena result = {0};
	result.arena = arena;
	result.arena_position = arena->position;
	return result;
}

void memory_arena_end_temp(Temp_Arena temp) {
	memory_arena_pop_to(temp.arena, temp.arena_position);
}
