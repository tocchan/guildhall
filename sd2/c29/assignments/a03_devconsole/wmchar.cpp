WndProc
{
	switch (msg) {
		case WM_CHAR: 
			wchar_t character = (wchar_t)wParam; 
			// g_theDevConsole->OnKeyPress( character ); 
			g_theInputSystem->PushCharacter( character ); 
			break; 
	}
}

// Input System Input Stream

class InputSystem
{
	public:
		InputSystem::EndFrame()
		{
			m_characters.clear(); 
		}

		void PushCharacter( char c ) 
		{
			m_characters.enqueue(c); 
		}

		bool PopCharacter( char* out )
		{
			if (have characters) {
				*out = m_characters.front(); 
				m_characters.pop(); 
				return true; 
			} else {
				return false; 
			}
		}

	public:
		// all key state
		// ...

		std::queue<char> m_characters; 
};


void DevConsole::Update()
{
	ProcessInput();
}

void DevConsole::ProcessInput()
{
	char c; 
	while (g_theInputSystem->PopCharacter(&c)) {
		AddCharacterToInput(c); 
	}
}