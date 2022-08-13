#pragma once

class STUUtils
{
public:
    template <typename T> static T* GetComponentByClass(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;

        auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};
