//
// Created by guppy on 4/23/26.
//

#pragma once
#include <concepts>


namespace gp::network
{
    template <typename F, typename T>
    concept EventCallback = std::invocable<F, T> && std::same_as<std::invoke_result_t<F, T>, void>; // concept slop
    // ^ got this from chatGPT. I wanted to use std::function, but it doesn't work well with templates :(

    // TODO: ByteBuffer type
}
