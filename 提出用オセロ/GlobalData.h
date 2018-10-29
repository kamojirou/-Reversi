#pragma once

enum class Result
{
	Win,
	Lose,
};


class GlobalData
{
private:
	static GlobalData * s_instance;
	Result m_result;
	//©•ª‚ÌF‚ğŒˆ‚ß‚é•Ï”
	char m_mycolor;    
	char m_blackStone;
	char m_whiteStone;

private:
	GlobalData();
	~GlobalData() = default;
	GlobalData(const GlobalData &) = delete;
	GlobalData(GlobalData &&) = delete;
	void operator =(const GlobalData &) = delete;
	void operator =(GlobalData &&) = delete;

public:
	static void Startup();
	static void Shutdown();
	static GlobalData * GetInstance();

	void SetMyColor(char mycolor);
	void SetBlackStone(char blackstone);
	void SetWhiteStone(char whitestone);

	char GetMyColor();
	char GetBlackStone();
	char GetWhiteStone();

	void SetResult(Result result);
	Result GetResult() const;
};