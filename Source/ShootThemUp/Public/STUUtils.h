// Shoot Them Up Game. All Rights Reserved.

#pragma once

class STUUtils
{
public:
    template <typename T>
    static T* GetComponentByClass(AActor* Actor)
    {
        if (!Actor) return nullptr;

        auto Component = Actor->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};
