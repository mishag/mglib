#ifndef INCLUDED_OBJECT_POOL_H
#define INCLUDED_OBJECT_POOL_H

#include <deque>
#include <memory>
#include <mutex>
#include <atomic>
#include <functional>
#include <utility>

namespace MG {

template <class VALUE_TYPE>
class mgc_DefaultPoolCreator {
public:
    std::unique_ptr<VALUE_TYPE> alloc()
    {
        return std::unique_ptr<VALUE_TYPE>(new VALUE_TYPE());
    }
};


template <class VALUE_TYPE>
class mgc_DefaultPoolResetter {
public:
    void reset(std::unique_ptr<VALUE_TYPE> & vp)
    {
        // this does nothing
    }
};

template <class VALUE_TYPE,
          class CREATOR  = mgc_DefaultPoolCreator<VALUE_TYPE>,
          class RESETTER = mgc_DefaultPoolResetter<VALUE_TYPE>>
class mgc_ObjectPool {

public:
    typedef std::unique_ptr<VALUE_TYPE> ValuePtr;

public:
    mgc_ObjectPool();
    ~mgc_ObjectPool();

    ValuePtr getObject();
    void returnObject(ValuePtr & obj);

    int numObjects() const
    {
        return d_numObjects;
    }

    int numAvailableObjects() const
    {
        return d_numAvailableObjects;
    }

private:
    // non-copyable, non-assignable
    mgc_ObjectPool(const mgc_ObjectPool &);
    mgc_ObjectPool & operator=(const mgc_ObjectPool &);

private:
    std::deque<ValuePtr>  d_pool;
    mutable std::mutex    d_mutex;
    std::atomic<int>      d_numAvailableObjects;
    std::atomic<int>      d_numObjects;
    CREATOR               d_creator;
    RESETTER              d_resetter;
};


//
// IMPLEMENTATION
//

template <class VALUE_TYPE,
          class CREATOR,
          class RESETTER>
mgc_ObjectPool<VALUE_TYPE,
               CREATOR,
               RESETTER>::mgc_ObjectPool() :
    d_numObjects(0),
    d_numAvailableObjects(0)
{
}

template <class VALUE_TYPE,
          class CREATOR,
          class RESETTER>
mgc_ObjectPool<VALUE_TYPE,
               CREATOR,
               RESETTER>::~mgc_ObjectPool()
{
}

template <class VALUE_TYPE,
          class  CREATOR,
          class RESETTER>
typename mgc_ObjectPool<VALUE_TYPE,
                        CREATOR,
                        RESETTER>::ValuePtr mgc_ObjectPool<VALUE_TYPE,
                                                           CREATOR,
                                                           RESETTER>::getObject()
{
    std::lock_guard<std::mutex> guard(d_mutex);

    // no objects left in the pool, create new one
    if (!d_numAvailableObjects) {
        ++d_numObjects;
        return d_creator.alloc();
    }

    // there are objects in the pool, take one and return
    ValuePtr vp = std::move(d_pool.front());
    d_pool.pop_front();
    --d_numAvailableObjects;

    return vp;
}

template <class VALUE_TYPE,
          class CREATOR,
          class RESETTER>
void mgc_ObjectPool<VALUE_TYPE,
                    CREATOR,
                    RESETTER>::returnObject(ValuePtr & vp)
{
    std::lock_guard<std::mutex> guard(d_mutex);

    d_resetter.reset(vp);
    d_pool.push_back(std::move(vp));
    ++d_numAvailableObjects;
}

} // namespace

#endif
