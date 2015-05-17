#include <hako/common/engine.h>
#include <hako/common/task.h>


void Hako::Engine::init()
{
    m_mem_alloc_callback  .init(this, &Engine::mem_alloc);
    m_mem_realloc_callback.init(this, &Engine::mem_realloc);
    m_mem_free_callback   .init(this, &Engine::mem_free);
	m_task_manager        .init(this);
}


void* Hako::Engine::mem_alloc(
	unsigned int size_in_bytes,
	unsigned int alignment_in_bytes)
{
	HAKO_UNUSED(alignment_in_bytes);
	return malloc(size_in_bytes);
}


void* Hako::Engine::mem_realloc(
	void* old_ptr,
	unsigned int old_size_in_bytes,
	unsigned int old_alignment_in_bytes,
	unsigned int new_size_in_bytes,
	unsigned int new_alignment_in_bytes)
{
	HAKO_UNUSED(old_size_in_bytes);
	HAKO_UNUSED(old_alignment_in_bytes);
	HAKO_UNUSED(new_alignment_in_bytes);
	return realloc(old_ptr, new_size_in_bytes);
}


void Hako::Engine::mem_free(
	void* ptr)
{
	free(ptr);
}