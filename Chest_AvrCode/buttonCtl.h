

/*

u8 Timeout
u8 NumSequences
struct Sequences
{
	u8 Momentary
	u8 NumCombos
	struct Combos
	{
		u8 NumButtons
		struct Buttons
		{
			u8 ButtonNum
		}
	}[NumCombos]
	u8 CommandLen
	u8 Command[CommandLen]
}[NumSequences]

*/