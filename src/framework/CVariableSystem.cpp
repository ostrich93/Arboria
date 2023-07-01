#include "Engine.h"
#include "CVariableSystem.h"
#include "../framework/String.h"
#include "../containers/HashIndex.h"

namespace Arboria {
	void CVariable::init(const char* _name, const char* _value, int _flags, const char* _description, float valueMin, float valueMax) {
		name = _name;
		value = _value;
		flags = _flags;
		description = _description;
		intValue = 0;
		floatValue = 0.0f;
		minValue = valueMin;
		maxValue = valueMax;
		internalVar = this;
		cVariableSystem->registerVariable(this);
	}

	class InternalCVariable : public CVariable {
		friend class CVariableSystemImpl;
	public:
		InternalCVariable() = default;
		InternalCVariable(const char* newName, const char* newValue, int newFlags);
		InternalCVariable(const CVariable* other);
		virtual ~InternalCVariable() = default;
		void update(const CVariable* other);
		void updateValue();
		void set(const char* newValue, bool force, bool isEngine);
		void reset();
	private:
		String nameString;
		String resetString;
		String valueString;
		String descriptionString;

		virtual void setStringInternal(const char* newValue);
		virtual void setBoolInternal(bool newValue);
		virtual void setIntegerInternal(int newValue);
		virtual void setFloatInternal(float newValue);
	};

	void InternalCVariable::setStringInternal(const char* newValue)
	{
		set(newValue, true, true);
	}
	void InternalCVariable::setBoolInternal(bool newValue)
	{
		set(String(newValue), true, false);
	}
	void InternalCVariable::setIntegerInternal(int newValue)
	{
		set(String(newValue), true, false);
	}
	void InternalCVariable::setFloatInternal(float newValue)
	{
		set(String(newValue), true, true);
	}

	InternalCVariable::InternalCVariable(const char* newName, const char* newValue, int newFlags)
	{
		nameString = newName;
		name = nameString.c_str();
		valueString = newValue;
		value = valueString.c_str();
		resetString = newValue;
		descriptionString = "";
		description = descriptionString.c_str();
		flags = (newFlags & ~CVariableFlags::CVAR_STATIC) | CVariableFlags::CVAR_MODIFIED;
		minValue = -1.f;
		maxValue = -1.f;
		updateValue();
		internalVar = this;
	}

	InternalCVariable::InternalCVariable(const CVariable* other)
	{
		nameString = other->getName();
		name = nameString.c_str();
		valueString = other->getString();
		value = valueString.c_str();
		resetString = other->getString();
		descriptionString = other->getString();
		description = descriptionString.c_str();
		flags = other->getFlags() | CVariableFlags::CVAR_MODIFIED;
		minValue = other->getMinValue();
		maxValue = other->getMaxValue();
		updateValue();
		internalVar = this;
	}

	void InternalCVariable::update(const CVariable* other)
	{
		flags |= other->getFlags();
		if (resetString.length() == 0)
			resetString = other->getString();
	}

	void InternalCVariable::updateValue()
	{
		bool clamped = false;
		if (flags & CVariableFlags::CVAR_BOOL) {
			intValue = (atoi(value) != 0);
			floatValue = intValue;
			if (String::iCompare(value, "0") != 0 && String::iCompare(value, "1") != 0) {
				valueString = String(bool(intValue != 0));
				value = valueString.c_str();
			}
		}
		else if (flags & CVariableFlags::CVAR_INTEGER) {
			intValue = atoi(value);
			if (minValue < maxValue) {
				if (intValue < minValue) {
					intValue = (int)minValue;
					clamped = true;
				}
				else if (intValue > maxValue) {
					intValue = (int)maxValue;
					clamped = true;
				}
			}
			if (clamped || !String::isNumeric(value) || !String::findChar(value, '.')) {
				valueString = String(intValue);
				value = valueString.c_str();
			}
			floatValue = (float)intValue;
		}
		else if (flags & CVariableFlags::CVAR_FLOAT) {
			floatValue = atof(value);
			if (minValue < maxValue) {
				if (floatValue < minValue) {
					floatValue = minValue;
					clamped = true;
				}
				else if (floatValue > maxValue) {
					floatValue = maxValue;
					clamped = true;
				}
			}
			if (clamped || String::isNumeric(value)) {
				valueString = String(floatValue);
				value = valueString.c_str();
			}
		}
		else {
			if (valueString.length() < 32) {
				floatValue = (float)atof(valueString.c_str());
				intValue = (int)floatValue;
			}
			else {
				floatValue = 0.0f;
				intValue = 0;
			}
		}
	}

	void InternalCVariable::set(const char* newValue, bool force, bool isEngine)
	{
		if (!newValue) {
			newValue = resetString.c_str();
		}

		if (!force) {
			if (flags & CVariableFlags::CVAR_READONLY) {
				Engine::printError("InternalCVariable::set: %s is read-only.\n", nameString.c_str());
				return;
			}
			if (flags & CVariableFlags::CVAR_CMDONLY) {
				Engine::printError("InternalCVariable::set: %s is write-protected.\n", nameString.c_str());
				return;
			}
		}

		if (valueString.iCompare(newValue) == 0) {
			return;
		}

		valueString = newValue;
		value = valueString.c_str();
		updateValue();
		setModified();
	}

	void InternalCVariable::reset()
	{
		valueString = resetString;
		value = valueString.c_str();
		updateValue();
	}

	class CVariableSystemImpl : public CVariableSystem {
		public:
			CVariableSystemImpl();
			virtual ~CVariableSystemImpl() {}
			virtual void init();
			virtual void shutdown();
			virtual bool isInitialized() const;
			virtual void registerVariable(CVariable* var);
			virtual CVariable* find(const char* name);
			virtual void setVariableString(const char* name, const char* value, int flags = 0);
			virtual void setVariableBool(const char* name, const bool value, int flags = 0);
			virtual void setVariableInteger(const char* name, const int value, int flags = 0);
			virtual void setVariableFloat(const char* name, const float value, int flags = 0);

			virtual void setModifiedFlags(int flags);
			virtual int getModifiedFlags() const;
			virtual void clearModifiedFlags(int flags);
			virtual void resetFlaggedVariables(int flags);

			virtual const char* getVariableString(const char* name) const;
			virtual bool getVariableBool(const char* name) const;
			virtual int getVariableInteger(const char* name) const;
			virtual float getVariableFloat(const char* name) const;

			InternalCVariable* findInternal(const char* name) const;
			void setInternal(const char* name, const char* value, int flags);

		private:
			bool initialized;
			List<InternalCVariable*> variables; //Use in conjunction with a hash index
			HashIndex hashIndex;
			int modifiedFlags; //used to keep track if a variable was changed or if something new was added.
	};

	CVariableSystemImpl implCVariableSystem;
	CVariableSystem* cVariableSystem = &implCVariableSystem;

	CVariableSystemImpl::CVariableSystemImpl()
	{
		initialized = false;
		modifiedFlags = 0;
	}

	void CVariableSystemImpl::init() {
		initialized = true;
	}

	void CVariableSystemImpl::shutdown() {
		variables.deleteContents(true);
		hashIndex.clearFree();
		initialized = false;
	}

	bool CVariableSystemImpl::isInitialized() const {
		return initialized;
	}

	void CVariableSystemImpl::registerVariable(CVariable* var) {
		InternalCVariable* _internal;
		var->setInternalVar(var);

		_internal = findInternal(var->getName());

		if (_internal) {
			_internal->update(var);
		}
		else {
			_internal = new InternalCVariable(var);
			int hash = hashIndex.generateKey(_internal->getName(), false);
			hashIndex.add(hash, variables.append(_internal));
		}

		var->setInternalVar(_internal);
	}

	CVariable* CVariableSystemImpl::find(const char* name) {
		return findInternal(name);
	}

	void CVariableSystemImpl::setVariableString(const char* name, const char* value, int flags) {
		setInternal(name, value, flags);
	}

	void CVariableSystemImpl::setVariableBool(const char* name, const bool value, int flags) {
		setInternal(name, String(value), flags);
	}

	void CVariableSystemImpl::setVariableInteger(const char* name, const int value, int flags) {
		setInternal(name, String(value), flags);
	}

	void CVariableSystemImpl::setVariableFloat(const char* name, const float value, int flags) {
		setInternal(name, String(value), flags);
	}

	void CVariableSystemImpl::setModifiedFlags(int flags) {
		modifiedFlags |= flags;
	}

	int CVariableSystemImpl::getModifiedFlags() const {
		return modifiedFlags;
	}

	void CVariableSystemImpl::clearModifiedFlags(int flags) {
		modifiedFlags &= ~flags;
	}

	void CVariableSystemImpl::resetFlaggedVariables(int flags) {
		for (int i = 0; i < variables.getLength(); i++) {
			InternalCVariable* cVariable = variables[i];
			if (cVariable->getFlags() & flags) {
				cVariable->set(NULL, true, true);
			}
		}
	}

	const char* CVariableSystemImpl::getVariableString(const char* name) const {
		InternalCVariable* _internal = findInternal(name);
		if (_internal) {
			return _internal->getString();
		}
		return "";
	}

	bool CVariableSystemImpl::getVariableBool(const char* name) const {
		InternalCVariable* _internal = findInternal(name);
		if (_internal) {
			return _internal->getBool();
		}
		return false;
	}

	int CVariableSystemImpl::getVariableInteger(const char* name) const {
		InternalCVariable* _internal = findInternal(name);
		if (_internal) {
			return _internal->getInteger();
		}
		return 0;
	}

	float CVariableSystemImpl::getVariableFloat(const char* name) const {
		InternalCVariable* _internal = findInternal(name);
		if (_internal) {
			return _internal->getFloat();
		}
		return 0.0f;
	}

	InternalCVariable* CVariableSystemImpl::findInternal(const char* name) const {
		int hash = hashIndex.generateKey(name, false);
		for (int i = hashIndex.first(hash); i != -1; hash = hashIndex.next(i)) {
			if (variables[i]->nameString.iCompare(name) == 0) {
				return variables[i];
			}
		}
		return NULL;
	}

	void CVariableSystemImpl::setInternal(const char* name, const char* value, int flags) {
		int hash;
		InternalCVariable* _internal = findInternal(name);
		if (_internal) {
			int readOnlyFlags = _internal->flags & CVariableFlags::CVAR_READONLY;
			int cmdOnly = _internal->flags & CVariableFlags::CVAR_CMDONLY;

			if (!(readOnlyFlags && cmdOnly)) {
				_internal->setStringInternal(value);
				_internal->flags |= flags & ~CVariableFlags::CVAR_STATIC;
			}
		}
		else {
			_internal = new InternalCVariable(name, value, flags);
			hash = hashIndex.generateKey(_internal->nameString.c_str(), false);
			hashIndex.add(hash, variables.append(_internal));
		}
	}
}