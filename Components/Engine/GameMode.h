//
// Created by Stefan Annell on 2022-01-29.
//

#pragma once
#include <string>


namespace voxie {
    class Scene;
    class Engine;

    class GameMode {
    public:
        GameMode();
        GameMode(const std::string &);
        virtual ~GameMode();

        // Called once
        virtual void Initialize();

        // Starts / stops tick
        virtual void Start();
        virtual void Stop();
        virtual void Resume();

        // On tick
        virtual void OnTick(float deltaTime);

        // Reloads scene
        virtual void Reset();

        bool IsStarted() const;

    protected:
        std::string sceneSavefile;
        int OnTickHandle = 0;
        bool started = false;
    };

}// namespace voxie
