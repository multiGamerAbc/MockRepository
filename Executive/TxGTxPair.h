#ifndef TXGTXPAIR_H
#define TXGTXPAIR_H

////////////////////////////////////////////////////////////////////////////////
// GTx		Class Declaration		(IGetTested   class   wannabe)
////////////////////////////////////////////////////////////////////////////////

class GTx {
public:

};

////////////////////////////////////////////////////////////////////////////////
// Tx		Class Declaration		(ITest		class wannabe)
////////////////////////////////////////////////////////////////////////////////

class Tx {
public:
	virtual bool test(GTx* obj) = 0;
};

#endif