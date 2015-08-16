
#define INPUT_UP 0
#define INPUT_DOWN 1
#define INPUT_LEFT 2
#define INPUT_RIGHT 3
#define INPUT_TURN_LEFT 4
#define INPUT_TURN_RIGHT 5
#define KEY_INPUTS 6

class ControlObject {
private:
	struct key {
		DWORD keyid;
		bool keydown;
		key(DWORD keyid_c = 0x20) {keyid = keyid_c; keydown = false;}
	} keyset[KEY_INPUTS];
	int getEntryNum (DWORD id);
public:
	bool updateKeyState (DWORD id, bool newstate);
	bool keyIsDown (unsigned num) {return keyset[num].keydown;}
	friend ControlObject defaultInputs ();
} input;


int ControlObject::getEntryNum (DWORD id) {
	for (unsigned i = 0; i < KEY_INPUTS; i++) 
		if (keyset[i].keyid == id)
			return i;
	return -1;
}

bool ControlObject::updateKeyState (DWORD id, bool newstate) {
	int EntryNum = getEntryNum(id);
	if (EntryNum == -1)
		return false;
	keyset[EntryNum].keydown = newstate;
	return true;
}

ControlObject defaultInputs () {
#define makeKey(x) ControlObject::key(x)
	ControlObject object;
	
	object.keyset[INPUT_UP] 		= makeKey(0x57); //W
    object.keyset[INPUT_DOWN] 		= makeKey(0x53); //S
    object.keyset[INPUT_LEFT] 		= makeKey(0x41); //A
    object.keyset[INPUT_RIGHT] 		= makeKey(0x44); //D
    object.keyset[INPUT_TURN_LEFT] 	= makeKey(0x51); //Q
    object.keyset[INPUT_TURN_RIGHT] = makeKey(0x45); //E
	
	return object;
#undef makeKey
}






struct GameInstance {
public:
	ControlObject input;
	camera playercam;
	//RECT* gameClient() {return &playercam.rec;}
	void update();
};

void GameInstance::update() {
	Tact::vector move (0.0,0.0);
	double dTilt = 0.0;
	if (input.keyIsDown(INPUT_UP)) move.y++;
	if (input.keyIsDown(INPUT_DOWN)) move.y--;
	if (input.keyIsDown(INPUT_LEFT)) move.x--;
	if (input.keyIsDown(INPUT_RIGHT)) move.x++;
	if (move.square() != 0) 
		move /= move.magnitude();
	
	if (input.keyIsDown(INPUT_TURN_LEFT)) dTilt++;
	if (input.keyIsDown(INPUT_TURN_RIGHT)) dTilt--;
	//move *= 0.001;
	playercam.focus += move;
	playercam.tilt += (dTilt)/20;

}


