#ifndef PANICPAINTER_PPTIMER_H
#define PANICPAINTER_PPTIMER_H

#include <cugl/cugl.h>
#include <cmath>
#include "PPTypeDefs.h"

namespace utils {
    /**
     * Timer is a useful function to do countdowns. Start by calling alloc(),
     * then start the countdown by calling update() each frame.
     * @author Dragonglass Studios
     */
    class Timer {
    private:
        /** The duration of the timer. */
        uint _duration;
        /** Time point when the timer started. */
        float _timeLeft;

    public:
        /** @deprecated Constructor. Use alloc() instead. */
        explicit Timer(uint duration) :
                _duration(duration), _timeLeft((float) duration) {}

        /**
         * Get a new instance of a timer.
         * @param duration In seconds.
         * @return A smart pointer of this timer.
         */
        static ptr<Timer> alloc(const uint duration) {
            ptr<Timer> result = std::make_shared<Timer>(duration);
            return result;
        }

        /** Whether the timer has started and has 0 seconds left. */
        bool finished() const;

        /** Reset the timer and stop it. */
        void reset();

        /** The duration of this timer. */
        uint getDuration() const;

        /** Time that is left. */
        uint timeLeft() const;

        /** Progress the timer. */
        void update(float timestep);
    };
}

#endif //PANICPAINTER_PPTIMER_H
