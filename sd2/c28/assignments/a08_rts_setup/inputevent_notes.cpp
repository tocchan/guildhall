//------------------------------------------------------------------------
class UIInputEvent
{
	public: 
		Vec2 m_cursorPostitionInUI; 
		int m_mouseButtonsDown; 
		int m_mouseButtonsUp; 

		bool m_isConsumed; 
};

//------------------------------------------------------------------------
class MouseEvent 
{
	Vec2 m_clientPosition; 
	int m_buttonsDown; 
	int m_buttonsUp;

	bool m_isConsumed; 
}; 

//------------------------------------------------------------------------
void App::TranslateMouseEventFromWindows( ...window args... )
{
	// Generate RAW event; 
	MouseEvent evt; 
	evt.clientPosition = g_theWindow->GetMousePosition(); 

	uint btnFlag = (1 << buttonIndex);
	if (up) {
		evt.buttonsUp = btnFlag; 
	} else {
		evt.buttonsDown = btnFlag;
	}

	App::HandleMouseEvent( evt ); 
}

//------------------------------------------------------------------------
void App::HandleMouseEvent( MouseEvent &evt ) 
{
	if (m_devConsole->isOpen()) {
		m_devConsole->HandleMouseEvent( evt ); 
		return; 
	}

	m_game->HandleMouseEvent( evt ); 
}

//------------------------------------------------------------------------
void Game::HandleMouseEvent( MouseEvent &evt )
{
	// UIWidget *m_pauseMenu; 
	if (m_pauseMenu->IsVisible()) {
		m_pauseMenu->HandleMouseEvent( &evt ); 
		if (evt.IsConsumed()) {
			return; 
		}
	}

	// Check if state wants to handle it
	if (m_state == GAME_STATE_EDITOR) {
		m_editorUI->HandleMouseEvent( &evt ); 
		if (!evt->IsConsumed()) {
			DoEditorSpecificInput( evt ); 
		}
	}

	// anything else?  No, finish; 
}

//------------------------------------------------------------------------
// UICanvas
void UIWidget::HandleMouseEvent( MouseEvent &evt ) 
{
	// InputEvent evt = TranslateMouseEvent( evt ); 
	UIInputEvent uiEvent; 
	uiEvent.cursorPositionInUI = m_uiCamera->ClientToWorldSpace( evt.clientPosition, 0.0f ).xy(); 
	uiEvent.mouseButtonsDown = evt.buttonsDown; 
	uiEvent.mouseButtonsUp = evt.buttonsUp; 

	HandleInput( uiEvent ); 

	if (uiEvent.IsConsumed()) {
		evt.Consume(); 
	}
}

//------------------------------------------------------------------------
void UIButton::HandleInput( UIInputEvent &evt )
{
	SetHover( m_worldBounds.Contains(evt.cursorPositionInUI) ); 

	if (IsHovered()) {
		if (!evt.IsConsumed() && evt.WasMouseButtonReleased(0)) {
			Click(); 
			evt.Consume(); 
		}
	}
}

//------------------------------------------------------------------------
void UIButton::Click() 
{
	OnMouseClockEvent.Trigger(); 
	EventFire( m_onClickEventString ); 
}

//------------------------------------------------------------------------
Game::SetupUI()
{
	UIWidget *menu = m_ui->CreateChild<UIWidget>(); 

	UILabelButton *play = menu->CreatChild<UILabelButton>();
	player->SetClickEvent( "play" ); 

	UILabelButton *quit = menu->CreatChild<UILabelButton>();
	quit->SetClockEvent( "quit" ); 
}