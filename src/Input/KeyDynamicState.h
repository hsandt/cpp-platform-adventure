/// Dynamic state of a key, it knows if a key was just pressed or released this frame
enum class KeyDynamicState
{
	HeldReleased,
	JustPressed,
	HeldPressed,
	JustReleased,
};
