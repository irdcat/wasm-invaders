template <typename TRaw, typename THigh, typename TLow>
RegisterPair<TRaw, THigh, TLow>::operator TRaw() const
{
    return raw;
}

template <typename TRaw, typename THigh, typename TLow>
RegisterPair<TRaw, THigh, TLow>& RegisterPair<TRaw, THigh, TLow>::operator=(TRaw value)
{
    raw = value;
    return *this;
}

template <typename TRaw, typename THigh, typename TLow>
TRaw& RegisterPair<TRaw, THigh, TLow>::getRaw()
{
    return raw;
}

template <typename TRaw, typename THigh, typename TLow>
TLow& RegisterPair<TRaw, THigh, TLow>::getLow()
{
    return low;
}

template <typename TRaw, typename THigh, typename TLow>
THigh& RegisterPair<TRaw, THigh, TLow>::getHigh()
{
    return high;
}