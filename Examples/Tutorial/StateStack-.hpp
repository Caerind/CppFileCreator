#ifndef BOOK_STATESTACK_HPP
#define BOOK_STATESTACK_HPP

#include <Book/State.hpp>
#include <Book/StateIdentifiers.hpp>
#include <Book/ResourceIdentifiers.hpp>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>


/* Here our first modification */
class sf::Event;
class sf::RenderWindow;

class StateStack : private sf::NonCopyable
{
	public:
		enum Action
		{
			Push,
			Pop,
			Clear,
		};


	public:		
		explicit			StateStack(State::Context context);
		
		
		/* Here our second modification */
		template <typename T> void registerState(States::ID stateID);

		void				update(sf::Time dt);
		void				draw();
		void				handleEvent(const sf::Event& event);

		void				pushState(States::ID stateID);
		void				popState();
		void				clearStates();

		bool				isEmpty() const;


	private:
		State::Ptr			createState(States::ID stateID);
		void				applyPendingChanges();


	private:
		struct PendingChange
		{
			explicit			PendingChange(Action action, States::ID stateID = States::None);

			Action				action;
			States::ID			stateID;
		};


	private:
		std::vector<State::Ptr>								mStack;
		std::vector<PendingChange>							mPendingList;

		State::Context										mContext;
		std::map<States::ID, std::function<State::Ptr()>>	mFactories;
};

/* Here the last modification : StateStack.inl may contains our template functions */
#include "StateStack.inl"
/* or you can just comment the template functions during the convertion */

#endif // BOOK_STATESTACK_HPP
