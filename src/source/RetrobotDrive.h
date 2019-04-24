void _driveFollowLine( int ticks, float speed, int breakCondition ){
	resetEncoders();
	int i;
	tick = 0;
	while (getEncoder(AVERAGE) < ticks)
	{
		i = followLineLoop(speed);
		if (!i)
		{	// Mistet strek
			LCDPrintf("Mistet strek - _dFLT");
			if( breakCondition ) break;
		}
	}
	stop();
}

void driveFollowLine( int ticks, float speed ){
	_driveFollowLine( ticks, speed, 1 );
}

/* Deprecated */
void driveFollowLineTicks(int tics)
{
	driveFollowLine( tics, 0.2 );
}

/* Deprecated */
void driveFollowLineTicksSlow(int tics)
{
	driveFollowLine( tics, 0.15 );
}

/* Deprecated */
void driveFollowLineTicksFast(int tics)
{
	driveFollowLine( tics, 0.3 );
}

void driveFollowLineTicksTrapp(int tics)
{
	_driveFollowLine( tics, 0.15, 0 );
	// resetEncoders();
	// int i;
	// tick = 0;
	// while (getEncoder(AVERAGE) < tics)
	// {
	// 	i = followLineLoop(0.15);
	// 	if (!i)
	// 	{	// Mistet strek
	// 		//stop();
	// 		LCDPrintf("Mistet strek - Trapp");
	// 		tick = 0;
	// 		//break;
	// 	}
	// }
	// stop();
}


void driveToCross(int retning, int tics)
{
	//LCDPrintf("%d", OSReadInLatch(0));
	if (retning == LEFT)
		retning = 0x78; //135; //248;
	else if (retning == RIGHT)
		retning = 0x1e; //240; //15;
	else
		retning = 0x7e; //128;

	resetEncoders();
	int i;
	while (getEncoder(AVERAGE) < tics && !(((~OSReadInLatch(0)) & retning) == retning))
	{
		i = followLineLoop(0.2);
		if (!i)
		{	// Mistet strek
			LCDPrintf("Mistet strek - dTCross");
			break;
		}
	}
	stop();
	if (getEncoder(AVERAGE) >= tics)
		LCDPrintf("\r\n Stoppet på tics");
	else
		LCDPrintf("\r\n Cross");
}

void driveToLine(int tics)
{
	resetEncoders();
	robotSpeed = 0.2;
	robotTurn = 0;
	driveRaw(robotSpeed, robotTurn);
	while (getEncoder(AVERAGE) < tics)
	{

		if (((~OSReadInLatch(0)) & 0x3c) > 0 ) //!binlys[3] && !binlys[4]
		{
			LCDPrintf("Line");
			break;
		}
	}
	stop();
}

void driveBlind( int ticks, float speed ){
	resetEncoders();
	driveRaw( speed, 0.0f );
	while( abs(getEncoder(AVERAGE)) < ticks );
	stop();
}

void fram(int tics)
{
	driveBlind( tics, 0.2f );
	// resetEncoders();
	// robotSpeed = 0.2f;
	// robotTurn = 0.0f;
	// driveRaw(robotSpeed, robotTurn);
	// while (getEncoder(AVERAGE) < tics) {;}
	// stop();
}

void framFast(int tics)
{
	driveBlind( tics, 0.25f );
	// resetEncoders();
	// robotSpeed = 0.25f;
	// robotTurn = 0.0f;
	// driveRaw(robotSpeed, robotTurn);
	// while (getEncoder(AVERAGE) < tics) {;}
	// stop();
}

void rygg(int tics)
{
	driveBlind( tics, -0.15f );
	// resetEncoders();
	// robotSpeed = -0.15f;
	// robotTurn = 0.0f;
	// driveRaw(robotSpeed, robotTurn);
	// while (getEncoder(AVERAGE) > -tics) {;}
	// stop();
}

void ryggFast(int tics)
{
	driveBlind( tics, -0.25f );
	// resetEncoders();
	// robotSpeed = -0.25f;
	// robotTurn = 0.0f;
	// driveRaw(robotSpeed, robotTurn);
	// while (getEncoder(AVERAGE) > -tics) {;}
	// stop();
}

void turn(int side, int tics)
{
	stop();
	resetEncoders();
	robotSpeed = 0.0;
	if (side == LEFT)
	{
		robotTurn = -0.3f;
		driveRaw(robotSpeed, robotTurn);
		while (getEncoder(side) > -tics) {;}
	}
	else
	{
		robotTurn = 0.3f;
		driveRaw(robotSpeed, robotTurn);
		while (getEncoder(side) > -tics) {;}
	}
	stop();
}

void turnToDistance(int sensor, int value)
{
	int last = getDistance(sensor);
	LCDPrintf("\r\n%d", last);
	robotTurn = 0.20f;
	robotSpeed = 0.0f;
	driveRaw(robotSpeed, robotTurn);
	while((last < value) || (last > (value + 100)))
	{
		last = getDistance(sensor);
		LCDPrintf("\r\n%d", last);
	}
	stop();
}

void turnToDistanceLeft(int sensor, int value)
{
	int last = getDistance(sensor);
	LCDPrintf("\r\n%d", last);
	robotTurn = -0.15f;
	robotSpeed = 0.0f;
	driveRaw(robotSpeed, robotTurn);
	while((last < value) || (last > (value + 100)))
	{
		last = getDistance(sensor);
		LCDPrintf("\r\n%d", last);
	}
	stop();
}

void driveToDistance(int sensor, int value)
{

	robotTurn = 0.0f;
	robotSpeed = 0.15f;
	driveRaw(robotSpeed, robotTurn);
	while(getDistance(sensor) < value)
	{
		LCDPrintf("\r\n%d", getDistance(sensor));
	}
	stop();
}

void ryggToDistance(int sensor, int value)
{
	int last = getDistance(sensor);
	LCDPrintf("\r\nR %d", last);
	robotTurn = 0.0f;
	robotSpeed = -0.15f;
	driveRaw(robotSpeed, robotTurn);
	while(last > value && last < (value + 100))
	{
		last = getDistance(sensor);
		LCDPrintf("\r\nR %d", last);
	}
	stop();
}

void waitUntilPathClear( int threshold ){
	for( int counter = 0; counter < 10; counter++ ){
		if( getDistance(FRONT) < threshold )
			counter++;
		else
			counter=0;
	}
}

void waitUntilPathBlocked( int threshold ){

	for( int counter = 0; counter < 10; counter++ ){
		if( getDistance(FRONT) > threshold )
			counter++;
		else
			counter=0;
	}
}

/*
void safeLostLine()
{
	// Rygg uten å resette enkoder
	int j = getEncoder(AVERAGE);
	robotSpeed = -0.15f;
	robotTurn = 0.0f;
	driveRaw(robotSpeed, robotTurn);
	while ((getEncoder(AVERAGE) - j) > -3000)
	{
		; // rygg til strek funnet
	}
	stop();
	// se etter strek
	// hvis strek fortsett
	// hvis ikke selfdestruct
}
*/
