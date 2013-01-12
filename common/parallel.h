#ifndef PARALLEL_H
#define PARALLEL_H

#include <QtCore>

namespace Parallel
{

template<typename T, typename Class>
class CParallelThread : public QThread
{
private:
	Class *m_object;
	T (Class::*m_method)();

public:
	explicit CParallelThread(Class *object, T (Class::*method)(), QObject *parent = 0) :
		QThread(parent)
	{
		m_object = object;
		m_method = method;
	}

	void run()
	{
		(m_object->*m_method)();
	}
};

template<typename T, typename Class>
CParallelThread<T, Class>* run(Class *object, T (Class::*method)())
{
	CParallelThread<T, Class> *parallelThread = new CParallelThread<T, Class>(object, method);
	if(parallelThread)
		parallelThread->start();
	return parallelThread;
}

}	//Parallel namespace

class CWaitThread : public QThread
{
public:
	static void msleep(unsigned long ms)
	{
		QThread::msleep(ms);
	}
};

#endif // PARALLEL_H
