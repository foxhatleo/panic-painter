#ifndef PANICPAINTER_PPTIMER_H
#define PANICPAINTER_PPTIMER_H

#include <cmath>
#include "PPHeader.h"

namespace utils {
    /**
     * Timer is a useful function to do countdowns. Start by calling alloc(),
     * then start the countdown by calling update() each frame.
     * @author Dragonglass Studios
     */
    class Timer {
    private:
        /** The duration of the timer. */
        float _duration;
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
        static ptr<Timer> alloc(const float duration) {
            ptr<Timer> result = make_shared<Timer>(duration);
            return result;
        }

        /** Whether the timer has started and has 0 seconds left. */
        bool finished() const;

        /** Reset the timer and stop it. */
        void reset();

        /** The duration of this timer. */
        float getDuration() const;

        /** Time that is left. */
        float timeLeft() const;

        /** Progress the timer. */
        void update(float timestep);

        string formatTime() const;
    };
}

#endif //PANICPAINTER_PPTIMER_H
