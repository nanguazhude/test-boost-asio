#ifndef __QTTHREAD_HPP
#define __QTTHREAD_HPP

#include <memory>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>
#include <functional>
#include <type_traits>
#include <QtCore/qdebug.h>
#include <QtCore/qobject.h>
#include <QtCore/qthread.h>
#include <QtCore/qvariant.h>
#include <QtCore/qeventloop.h>
#include <QtCore/qcoreevent.h> 

template<typename $T$Object>class QtThread;

class QtThreadVoidQObjectData : public std::shared_ptr<const void> {
	using $T$Super = std::shared_ptr<const void>;
public:
	template<typename ... Args>
	QtThreadVoidQObjectData(Args && ... args) :$T$Super(std::forward<Args>(args)...) {}
	/*********************************************************************************/
	template<typename ... Args>
	static inline QVariant cast(Args && ... args) {
		return QVariant::fromValue<QtThreadVoidQObjectData>(QtThreadVoidQObjectData{ std::forward<Args>(args)... });
	}
};

class $T$QtThread {
public:

	class RunEvent : public QEvent {
	public:
		RunEvent() :QEvent(QEvent::MaxUser) {}
		template<typename T, typename ... Args>
		RunEvent(T && a0, Args && ...args) :
			$m$RunFunction(std::forward<T>(a0), std::forward<Args>(args)...),
			RunEvent() {}
		std::function<void(void)> $m$RunFunction;
	};

	class RunObject : public QObject {
	public:
		virtual bool event(QEvent *e) {
			RunEvent * r = dynamic_cast<RunEvent*>(e);
			if (r) {
				try { if (r->$m$RunFunction) { (r->$m$RunFunction)(); } }
				catch (...) {}
				return true;
			}
			return QObject::event(e);
		}
		void postEvent(std::function<void(void)>);
	};

	class $T$Data {
		friend class $T$QtThread;
		template<typename> friend class QtThread;
	protected:
		std::shared_ptr<QEventLoopLocker> $m$Locker;
		std::atomic<bool> $m$IsQuit{ false };
		RunObject $m$RunObject;
	public:
		virtual ~$T$Data() = default;
		void clearThreadLocker() { $m$Locker = {}; }
		void setThreadLock(QThread * arg) { $m$Locker = std::make_shared<QEventLoopLocker>(arg); }
		bool isQuit() const { return $m$IsQuit.load(); }
		bool setQuit() {
			bool exp = false;
			$m$IsQuit.compare_exchange_strong(exp, true);
			return exp;
		}
		template<typename Arg>
		void runInThread(Arg && arg) { $m$RunObject.postEvent(std::forward<Arg>(arg)); }
	};

	std::weak_ptr<$T$Data> $m$Watcher;
	static QCoreApplication * getQApp();
	static QThread * getQAppThread();
	static void connnectQAppQuit(QObject*, std::function<void(void)>);

	$T$QtThread() = default;
	$T$QtThread($T$QtThread&&) = default;
	$T$QtThread(const $T$QtThread &) = default;
	$T$QtThread&operator=($T$QtThread&&) = default;
	$T$QtThread&operator=(const $T$QtThread &) = default;
	virtual ~$T$QtThread() = default;

	virtual void start() = 0;
	virtual void quit() const = 0;
	virtual bool isQuit() const = 0;
	virtual QVariant getLockVariant() const = 0;
	static inline constexpr const char * getLockPropertyName() { return "\xFF" "P" "\xFE" ""; }
	virtual bool lock(QObject * arg) const = 0;
	static inline constexpr const char * getQtThreadName() { return "\xFA" "Q" "\xFB" ""; }
	static std::shared_ptr<$T$QtThread> getQtThread(QObject * arg) {
		if (arg == nullptr) { return {}; }
		auto varAnsV = arg->property(getQtThreadName());
		if (varAnsV.isValid()) {
			auto varAnsVoid = varAnsV.value<QtThreadVoidQObjectData>();
			if (varAnsVoid) {
				return std::shared_ptr<$T$QtThread>(varAnsVoid,
					static_cast<$T$QtThread*>(const_cast<void *>(varAnsVoid.get())));
			}
		}
	}
};

template<typename $T$Object/*$T$Object必须有默认构造函数，并且不能抛出异常*/>
class QtThread :public $T$QtThread {
	//using $T$Object = QObject;
	class $T$DataPrivate : public $T$Data {
	public:
		$T$Object $m$Object/*默认构造函数*/;
	};

	class $T$QThread : public QThread {
	public:
		virtual void run() override {
			auto varWatcher = std::make_shared<$T$DataPrivate>()/*在此线程创建数据*/;
			$m$Promise->set_value(varWatcher)/*通知数据创建完成*/;
			$m$Promise = nullptr;
			/******************************************************/
			//进行事件循环
			try { this->exec(); }
			catch (...) {}
			/******************************************************/
			//事件循环完成后收尾
			varWatcher->setQuit();
			using namespace std::chrono_literals;
			while (varWatcher.use_count() > 1) {
				qDebug() << "thread should be destoryed , but data used !!!";
				std::this_thread::sleep_for(1s);
			}
		}
		std::promise< std::weak_ptr<$T$DataPrivate> > * $m$Promise;
	};

	class ThisPrivateData : public  std::atomic<$T$QThread *> {
		using _x_super = std::atomic<$T$QThread *>;
	public:
		ThisPrivateData($T$QThread *arg) :_x_super(arg) {}
	};

	std::shared_ptr<ThisPrivateData> $m$QThread{ new ThisPrivateData{nullptr} };

public:

	QtThread() = default;
	~QtThread() = default;
	QtThread(const QtThread & arg) = default;
	QtThread(QtThread &&) = default;
	QtThread&operator=(const QtThread &) = default;
	QtThread&operator=(QtThread &&) = default;

	inline void start() override {
		{
			$T$QThread * exp = nullptr;
			$m$QThread->compare_exchange_strong(exp, reinterpret_cast<$T$QThread*>(128u));
			if (exp) { return; }
		}

		$T$QThread * argQThread = new $T$QThread;
		$m$QThread->store(argQThread);
		argQThread->moveToThread(getQAppThread())/*移动到主线程*/;
		argQThread->connect(argQThread, &QThread::finished,
			argQThread, &QThread::deleteLater)/*完成后自销毁*/;

		{/*初始化私有数据*/
			std::promise< std::weak_ptr<$T$DataPrivate>/**/> varPromise;
			argQThread->$m$Promise = &varPromise;
			auto varFuture = varPromise.get_future();
			argQThread->start()/*启动线程*/;
			this->$m$Watcher = varFuture.get();
		}

		{/*进行线程自锁*/
			auto varWatcher = this->$m$Watcher.lock();
			varWatcher->setThreadLock(argQThread);
		}

		{
			std::shared_ptr<$T$QtThread> varThisThread = std::make_shared<QtThread>(*this);
			argQThread->setProperty(getQtThreadName(), QtThreadVoidQObjectData::cast(varThisThread));
			if constexpr(std::is_base_of_v<QObject, std::remove_cv_t< std::remove_reference_t<$T$Object > > >) {
				auto varWatcher = this->$m$Watcher.lock();
				varWatcher->$m$RunObject.postEvent([varWatcher, varThisThread]() {
					auto var_thisp = static_cast<$T$DataPrivate*>(varWatcher.get());
					var_thisp->$m$Object.
						setProperty(getQtThreadName(), QtThreadVoidQObjectData::cast(varThisThread));
					var_thisp->$m$RunObject.
						setProperty(getQtThreadName(), QtThreadVoidQObjectData::cast(varThisThread));
				});
			}
			else {
				auto varWatcher = this->$m$Watcher.lock();
				varWatcher->$m$RunObject.postEvent([varWatcher, varThisThread]() {
					auto var_thisp = static_cast<$T$DataPrivate*>(varWatcher.get());
					var_thisp->$m$RunObject.
						setProperty(getQtThreadName(), QtThreadVoidQObjectData::cast(varThisThread));
				});
			}
		}

		{/*qt application 退出通知*/
			connnectQAppQuit(argQThread, [var = *this]() mutable { var.stop<true>(); });
		}

	}

	template<typename bool ForceToQuit = false >
	inline void stop() const {
		try {
			auto varWatcher = this->$m$Watcher.lock();
			if (varWatcher) {
				if constexpr(ForceToQuit) {
					varWatcher->setQuit()/*设置退出标志位*/;
					varWatcher->clearThreadLocker()/*取消自锁*/;
					varWatcher = {}/*取消本函数引用的监视器*/;
					$m$QThread->load()->quit()/*quit now!!!*/;
					return;
				}
				if (varWatcher->setQuit()) return;
				varWatcher->clearThreadLocker()/*取消自锁*/;
			}
		}
		catch (...) {}
	}

	inline void quit() const override { return this->stop<false>(); }

	std::shared_ptr<$T$Object> object() const {
		try {
			auto varWatcher = this->$m$Watcher.lock();
			if (varWatcher) {
				if (varWatcher->isQuit()) return{};
				auto varPointer = &((static_cast<$T$DataPrivate *>(varWatcher.get()))->$m$Object);
				return { varWatcher ,varPointer };
			}
		}
		catch (...) {}
		return {};
	}

	bool isQuit() const override {
		try {
			const auto varWatcher = this->$m$Watcher.lock();
			if (varWatcher) {
				return varWatcher->isQuit();
			}
		}
		catch (...) {}
		return true;
	}

	QVariant getLockVariant() const override {
		try {
			const auto varWatcher = this->$m$Watcher.lock();
			if (varWatcher) {
				if (varWatcher->isQuit()) return{};
				QtThreadVoidQObjectData::cast(varWatcher->$m$Locker);
			}
		}
		catch (...) {}
		return{};
	}

	inline bool lock(QObject * arg) const override {
		const QVariant varLock = getLockVariant();
		if ((varLock.isNull()) || (!varLock.isValid())) { return false; }
		arg->setProperty(getLockPropertyName(), varLock);
		arg->moveToThread($m$QThread->load());
		return true;
	}

	inline void runInThread(std::function<void(void)> arg){
		try {
			const auto varWatcher = this->$m$Watcher.lock();
			if (varWatcher) {
				if (varWatcher->isQuit()) return{};
				varWatcher->$m$RunObject.postEvent(std::move(arg));
			}
		}
		catch (...) {}
	}

};

Q_DECLARE_METATYPE(QtThreadVoidQObjectData)

#endif

