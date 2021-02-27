/// Dynamic state of a key. It defines if a key is pressed or released,
/// and for how many frames it has been. This allows to determine if
/// a key has just been pressed or released.
struct KeyDynamicState
{
    /// Is the key currently pressed?
    bool isPressed;

    /// The number of frames elapsed since it was pressed or released last.
    /// It is incremented during key dynamic states update.
    /// 0: the KeyPressed/KeyReleased event has just been processed during event polling,
    ///    and not confirmed yet.
    /// 1: the KeyPressed/KeyReleased event has been confirmed, and we consider the key to
    ///    have been "just pressed/released" this frame.
    /// 2-255: after being just pressed/released, the key remained in this state for 1
    ///    frame or more. 255 frames @ 60 FPS ~ 4 seconds, enough for most actions.
    /// The only reason we have both 0 and 1 for "just pressed/released" values is that
    /// we have two input passes: event poll loop (only processes actual press/release)
    /// and key dynamic states update (only handles holding same state), and we need to
    /// distinguish a key just pressed/released this frame (0) from a key just pressed/
    /// released last frame (1) so only the latter becomes "held" if it keeps the same
    /// state this frame.
    u8 framesSinceLastStateChange;
};
