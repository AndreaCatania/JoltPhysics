// Jolt Physics Library (https://github.com/jrouwe/JoltPhysics)
// SPDX-FileCopyrightText: 2021 Jorrit Rouwe
// SPDX-License-Identifier: MIT

#pragma once

#include <Jolt/Core/StreamIn.h>
#include <Jolt/Core/StreamOut.h>

JPH_NAMESPACE_BEGIN

class Body;
class Constraint;
class BodyID;

/// Class that records the state of a physics system. Can be used to check if the simulation is deterministic by putting the recorder in validation mode.
/// Can be used to restore the state to an earlier point in time.
class JPH_EXPORT StateRecorder : public StreamIn, public StreamOut
{
public:
	/// Constructor
						StateRecorder() = default;
						StateRecorder(const StateRecorder &inRHS)					: mIsValidating(inRHS.mIsValidating) { }

	/// Sets the stream in validation mode. In this case the physics system ensures that before it calls ReadBytes that it will
	/// ensure that those bytes contain the current state. This makes it possible to step and save the state, restore to the previous
	/// step and step again and when the recorded state is not the same it can restore the expected state and any byte that changes
	/// due to a ReadBytes function can be caught to find out which part of the simulation is not deterministic
	void				SetValidating(bool inValidating)							{ mIsValidating = inValidating; }
	bool				IsValidating() const										{ return mIsValidating; }

	/// User callbacks that allow determining which parts of the simulation should be saved
	/// Note that these functions will be called repeatedly for the same object(s) and should consistently return the same value or the saved state will be corrupt
	virtual bool		ShouldSaveBody(const Body &inBody) const					{ return true; }
	virtual bool		ShouldSaveConstraint(const Constraint &inConstraint) const	{ return true; }
	virtual bool		ShouldSaveContact(const BodyID &inBody1, const BodyID &inBody2) const { return true; }

private:
	bool				mIsValidating = false;
};

JPH_NAMESPACE_END
