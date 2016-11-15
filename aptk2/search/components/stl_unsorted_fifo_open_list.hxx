
/*
Lightweight Automated Planning Toolkit
Copyright (C) 2015

<contributors>
Miquel Ramirez <miquel.ramirez@rmit.edu.au>
</contributors>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <aptk2/search/interfaces/open_list.hxx>
#include "true_evaluator.hxx"
#include <deque>
#include <cassert>

namespace aptk {

	template < typename NodeType, typename EvaluatorT=TrueEvaluator<NodeType> >
	class StlUnsortedFIFO : public OpenList< NodeType, std::deque< std::shared_ptr<NodeType> > > {
	public:
		typedef std::shared_ptr< NodeType > NodePtrType;

		//! The default constructor constructs a default evaluator
		StlUnsortedFIFO()
			: _evaluator( nullptr )
		{}

		//! This constructor allows the evaluator object to be injected
		StlUnsortedFIFO(std::shared_ptr<EvaluatorT> evaluator)
			: _evaluator(evaluator)
		{
			assert(_evaluator != nullptr);
		}

		virtual ~StlUnsortedFIFO() { }

		bool insert(const NodePtrType& n) override {
			if ( _evaluator != nullptr && !_evaluator->accept( *n ) )
				return false;

			this->push_back( n );
			return true;
		}
		
		bool updatable(const NodePtrType& node) { return false; }

		virtual NodePtrType get_next( ) override {
			assert( !is_empty() );
			NodePtrType next = this->front();
			this->pop_front();
			return next;
		}

		virtual bool is_empty() const override {
			return this->empty();
		}

	protected:
		//! The acceptor object, which is required to have a single method 'bool accept(NodeType)'
		std::shared_ptr<EvaluatorT> _evaluator;
	};

}
