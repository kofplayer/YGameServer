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
        addFilter(filter1);
    }
    EventFilterBase(T filter1, T filter2)
    {
        addFilter(filter1);
        addFilter(filter2);
    }
    EventFilterBase(T filter1, T filter2, T filter3)
    {
        addFilter(filter1);
        addFilter(filter2);
        addFilter(filter3);
    }
    EventFilterBase(T filter1, T filter2, T filter3, T filter4)
    {
        addFilter(filter1);
        addFilter(filter2);
        addFilter(filter3);
        addFilter(filter4);
    }
    void addFilter(T filter)
    {
        m_filterList.push_back(filter);
    }
    uint32 getFilterCount() const
    {
        return (uint32)m_filterList.size();
    }
    const T  getFilter(uint8 index) const
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