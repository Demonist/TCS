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

/**
Фукнция для запуска метода в новом потоке.
\param object определяет объект у которого будет вызван метод
\param method метод объекта \a object который будет вызван в новом, созданном потоке.

Данная функция аналогична QtConcurrent::run, но в отличии от нее не ожидает освобождения потока в пуле, а всегда создает новый.

\note Для избежания утечки памяти, удаляйте созданный поток.

Примеры использования:
\code
Parallel::run(this, &MyClass::doWork);

QThread *thread = Parallel::run(this, &MyClass::doWork);
...
delete thread;
\endcode
*/
template<typename T, typename Class>
CParallelThread<T, Class>* run(Class *object, T (Class::*method)())
{
	CParallelThread<T, Class> *parallelThread = new CParallelThread<T, Class>(object, method);
	if(parallelThread)
		parallelThread->start();
	return parallelThread;
}

/**
Функция возвращает новый созданный поток, запустив который вызовется метод \a method у объекта \a object.
\param object определяет объект у которого будет вызван метод
\param method метод объекта \a object который будет вызван в новом, созданном потоке.

\note Для избежания утечки памяти, удаляйте созданный поток.

Пример использования:
\code
QThread *thread = Parallel::runStoped(this, &MyClass::doWork);
QObject anyObject;	//Объект, который будет использоваться в методе doWork и который нужно перенести в новый поток.
anyObject.moveToThread(thread);
thread->start();	//В этот момент произойдет вызов метода doWork.
...
delete thread;
\endcode

\see CParallelThread<T, Class>* run(Class *object, T (Class::*method)())
*/
template<typename T, typename Class>
CParallelThread<T, Class>* runStoped(Class *object, T (Class::*method)())
{
	return new CParallelThread<T, Class>(object, method);
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
