YGAME_SERVER_BEGIN

template<typename T>
class EventFilterBase
{
public:
    EventFilterBase()
    {
    }
    EventFilterBase(T filter1)
    {
        AddFilter(filter1);
    }
    EventFilterBase(T filter1, T filter2)
    {
        AddFilter(filter1);
        AddFilter(filter2);
    }
    EventFilterBase(T filter1, T filter2, T filter3)
    {
        AddFilter(filter1);
        AddFilter(filter2);
        AddFilter(filter3);
    }
    EventFilterBase(T filter1, T filter2, T filter3, T filter4)
    {
        AddFilter(filter1);
        AddFilter(filter2);
        AddFilter(filter3);
        AddFilter(filter4);
    }
    void AddFilter(T filter)
    {
        m_filterList.push_back(filter);
    }
    uint32 GetFilterCount() const
    {
        return (uint32)m_filterList.size();
    }
    const T  GetFilter(uint8 index) const
    {
        if (index >= m_filterList.size())
        {
            return 0;
        }
        return m_filterList[index];
    }
    bool operator==(const EventFilterBase<T> &v)const
    {
        return v.m_filterList == m_filterList;
    }
private:
    YVector<T> m_filterList;
};

typedef EventFilterBase<uint32> EventFilter;

YGAME_SERVER_END