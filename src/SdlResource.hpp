#pragma once

#include <memory>
#include <functional>
#include <system_error>

#include <SDL2/SDL.h>

#define TYPENAME_STRING(type) #type

template<typename Creator, typename Destructor, typename... Args>
auto make_sdl_resource(Creator creator, Destructor destructor, Args&&... args)
{
    auto resource = creator(std::forward<Args>(args)...);
    if(!resource)
        throw std::system_error(
            errno, 
            std::generic_category(), 
            std::string("Could not create") + std::string(TYPENAME_STRING(decltype(*resource))) + std::string(SDL_GetError())
            );
    return std::unique_ptr<std::decay_t<decltype(*resource)>, decltype(destructor)>(resource, destructor);
}

template <typename T>
using SdlDeleter = std::function<void(T*)>;

template <typename T>
using SdlResource = std::unique_ptr<T, SdlDeleter<T>>;