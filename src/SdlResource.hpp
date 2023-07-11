#pragma once

#include <memory>
#include <functional>
#include <SDL2/SDL.h>

template<typename Creator, typename Destructor, typename... Args>
auto make_sdl_resource(Creator creator, Destructor destructor, Args&&... args)
{
    auto resource = creator(std::forward<Args>(args)...);
    if(!resource) {
        return std::unique_ptr<std::decay_t<decltype(*resource)>, decltype(destructor)>(nullptr, destructor);
    }
    return std::unique_ptr<std::decay_t<decltype(*resource)>, decltype(destructor)>(resource, destructor);
}

template<typename Creator, typename Destructor, typename Arg>
auto make_sdl_resource(Creator creator, Destructor destructor, Arg arg) 
{
    auto resource = creator(std::forward<Arg>(arg));
    if(!resource) {
        return std::unique_ptr<std::decay_t<decltype(*resource)>, decltype(destructor)>(nullptr, destructor);
    }
    return std::unique_ptr<std::decay_t<decltype(*resource)>, decltype(destructor)>(resource, destructor);
}

template <typename T>
using SdlDeleter = std::function<void(T*)>;

template <typename T>
using SdlResource = std::unique_ptr<T, SdlDeleter<T>>;