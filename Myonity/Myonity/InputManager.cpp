#include "pch.h"
#include "InputManager.h"

InputManager::InputManager()
	: m_keyCode{
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,
		VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6,
		VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
		'1','2','3','4','5','6','7','8','9','0',
		'Q','W','E','R','T','Y','U','I','O','P',
		'A','S','D','F','G','H','J','K','L',
		'Z','X','C','V','B','N','M',

		VK_MENU,    // ALT,
		VK_CONTROL, // CTRL,
		VK_LSHIFT,  // LSHIFT,
		VK_SPACE,   // SPACE,
		VK_RETURN,  // ENTER,
		VK_ESCAPE,  // ESC,
		VK_TAB,     // TAB

		VK_LBUTTON, // LBTN
		VK_RBUTTON, // RBTN
	}
{

}

InputManager::~InputManager()
{

}

void InputManager::Initialize()
{
	for(int i = 0; i < (int)KEY::LAST; i++)
	{
		m_keyInfos.push_back(KeyInfo{ KEY_STATE::NONE, false });
	}
}

void InputManager::Update()
{
	//������ ��Ŀ�� �˾Ƴ���
	HWND hWnd = GetFocus();

	//�����찡 ��Ŀ�� ���� ��
	if(hWnd != nullptr)
	{
		for(int i = 0; i < (int)KEY::LAST; i++)
		{
			//Ű�� �����ִ�.
			if(GetAsyncKeyState(m_keyCode[i]) & 0x8000)
			{
				if(m_keyInfos[i].isPrevPush)
				{
					// �������� ���� �־���.
					m_keyInfos[i].state = KEY_STATE::HOLD;
				}
				else
				{
					m_keyInfos[i].state = KEY_STATE::DOWN;
				}
				m_keyInfos[i].isPrevPush = true;
			}
			//Ű�� �������� �ʴ�.
			else
			{
				if(m_keyInfos[i].isPrevPush)
				{
					m_keyInfos[i].state = KEY_STATE::UP;
				}
				else
				{
					m_keyInfos[i].state = KEY_STATE::NONE;
				}

				m_keyInfos[i].isPrevPush = false;
			}
		}
	}

	//�����찡 ��Ŀ�� ���� �ƴ� ��,
	else
	{
		for(int i = 0; i < (int)KEY::LAST; i++)
		{
			m_keyInfos[i].isPrevPush = false;

			if(m_keyInfos[i].state == KEY_STATE::DOWN || m_keyInfos[i].state == KEY_STATE::HOLD)
			{
				m_keyInfos[i].state = KEY_STATE::UP;
				m_keyInfos[i].isPrevPush = true;
			}
			else if(m_keyInfos[i].state == KEY_STATE::UP)
			{
				m_keyInfos[i].state = KEY_STATE::NONE;
				m_keyInfos[i].isPrevPush = false;
			}
		}
	}
}
