#ifndef PANICPAINTER_PPTIMER_H
#define PANICPAINTER_PPTIMER_H

#include <cmath>
#include "PPHeader.h"

namespace utils {
    /**
     * Timer is a useful function to do countdowns. Start by calling to(),
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
        /** @deprecated Constructor. Use to() instead. */
        explicit Timer(float duration) :
            _duration(duration), _timeLeft(duration) {
            CUAssertLog(duration > 0, "Duration must be positive.");
        }

        /**
         * Get a new instance of a timer.
         * @param duration In seconds.
         * @return A smart pointer of this timer.
         */
        static ptr<Timer> alloc(const float duration) {
            ptr<Timer> result = make_shared<Timer>(duration);
            return result;
        }

        /** Whether the timer has run out. */
        bool finished() const { return timeLeft() <= 0; }

        /** Reset the timer. */
        void reset() { _timeLeft = _duration; }

        /** The duration of this timer. */
        float getDuration() const { return _duration; }

        /** Time that is left. */
        float timeLeft() const { return _timeLeft < 0.01f ? 0 : _timeLeft; }

        /** Progress the timer. */
        void update(float timestep);

        string formatTime() const;
    };
}

#endif //PANICPAINTER_PPTIMER_H
