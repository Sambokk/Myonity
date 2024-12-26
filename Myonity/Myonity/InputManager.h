#pragma once

enum class KEY_STATE
{
	DOWN, //이전X 현재O
	HOLD, //이전O 현재O
	UP,   //이전O 현재X
	NONE  //이전X 현재X
};

enum class KEY
{
	LEFT, RIGHT, UP, DOWN,
	F1, F2, F3, F4, F5, F6,
	F7, F8, F9, F10, F11, F12,
	NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9, NUM0,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,

	ALT, CTRL, LSHIFT,
	SPACE, ENTER, ESC, TAB,

	LMOUSE, RMOUSE,

	LAST, //enum의 끝
};

struct KeyInfo
{
	KEY_STATE state; // 키의 상태값
	bool      isPrevPush;  // 이전 프레임에서 눌렸는지 여부
};

class InputManager
{
	/// 생성자와 소멸자 ---------------------------------------------------
public:
	InputManager();
	~InputManager();

	/// 맴버 변수 --------------------------------------------------------
private:
	vector<KeyInfo> m_keyInfos;
	int m_keyCode[(int)KEY::LAST];

	/// 맴버 함수 --------------------------------------------------------
public:
	void Initialize();
	void Update();

	KEY_STATE GetKeyState(KEY _key) { return m_keyInfos[(int)_key].state; }
	bool IsKeyDown(KEY _key) { return m_keyInfos[(int)_key].state == KEY_STATE::DOWN; }
	bool IsKeyHold(KEY _key) { return m_keyInfos[(int)_key].state == KEY_STATE::HOLD; }
	bool IsKeyUp(KEY _key) { return m_keyInfos[(int)_key].state == KEY_STATE::UP; }
	bool IsKeyNone(KEY _key) { return m_keyInfos[(int)_key].state == KEY_STATE::NONE; }
};