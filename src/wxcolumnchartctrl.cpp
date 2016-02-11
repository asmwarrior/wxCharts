/*
	Copyright (c) 2016 Xavier Leclercq

	Permission is hereby granted, free of charge, to any person obtaining a
	copy of this software and associated documentation files (the "Software"),
	to deal in the Software without restriction, including without limitation
	the rights to use, copy, modify, merge, publish, distribute, sublicense,
	and/or sell copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
	IN THE SOFTWARE.
*/

/*
	Part of this file were copied from the Chart.js project (http://chartjs.org/)
	and translated into C++.

	The files of the Chart.js project have the following copyright and license.

	Copyright (c) 2013-2016 Nick Downie
	Released under the MIT license
	https://github.com/nnnick/Chart.js/blob/master/LICENSE.md
*/

#include "wxcolumnchartctrl.h"
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

wxColumnChartDataset::wxColumnChartDataset(const wxColor &fillColor,
										   const wxColor &strokeColor,
										   const wxVector<wxDouble> &data)
	: m_fillColor(fillColor), m_strokeColor(strokeColor), m_data(data)
{
}

const wxColor& wxColumnChartDataset::GetFillColor() const
{
	return m_fillColor;
}

const wxColor& wxColumnChartDataset::GetStrokeColor() const
{
	return m_strokeColor;
}

const wxVector<wxDouble>& wxColumnChartDataset::GetData() const
{
	return m_data;
}

wxColumnChartData::wxColumnChartData(const wxVector<wxString> &labels)
	: m_labels(labels)
{
}

void wxColumnChartData::AddDataset(wxColumnChartDataset::ptr dataset)
{
	m_datasets.push_back(dataset);
}

const wxVector<wxString>& wxColumnChartData::GetLabels() const
{
	return m_labels;
}

const wxVector<wxColumnChartDataset::ptr>& wxColumnChartData::GetDatasets() const
{
	return m_datasets;
}

wxColumnChartCtrl::Column::Column(wxDouble value,
								  wxDouble x,
								  wxDouble y,
								  const wxColor &fillColor,
								  const wxColor &strokeColor)
	: wxChartRectangle(x, y, wxChartRectangleOptions(fillColor, strokeColor)),
	m_value(value)
{
}

wxDouble wxColumnChartCtrl::Column::GetValue() const
{
	return m_value;
}

wxColumnChartCtrl::Dataset::Dataset()
{
}

const wxVector<wxColumnChartCtrl::Column::ptr>& wxColumnChartCtrl::Dataset::GetColumns() const
{
	return m_columns;
}

void wxColumnChartCtrl::Dataset::AppendColumn(Column::ptr column)
{
	m_columns.push_back(column);
}

wxColumnChartCtrl::wxColumnChartCtrl(wxWindow *parent,
									 wxWindowID id,
									 const wxColumnChartData &data,
									 const wxPoint &pos,
									 const wxSize &size,
									 long style)
	: wxChartCtrl(parent, id, pos, size, style), 
	m_grid(size, data.GetLabels(), GetMinValue(data.GetDatasets()),
		GetMaxValue(data.GetDatasets()), m_options.GetGridOptions())
{
	const wxVector<wxColumnChartDataset::ptr>& datasets = data.GetDatasets();
	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxColumnChartDataset& dataset = *datasets[i];
		Dataset::ptr newDataset(new Dataset());

		const wxVector<wxDouble>& data = dataset.GetData();
		for (size_t j = 0; j < data.size(); ++j)
		{
			newDataset->AppendColumn(Column::ptr(new Column(data[j], 25, 50, dataset.GetFillColor(), dataset.GetStrokeColor())));
		}

		m_datasets.push_back(newDataset);
	}
}

const wxColumnChartOptions& wxColumnChartCtrl::GetOptions() const
{
	return m_options;
}

wxDouble wxColumnChartCtrl::GetMinValue(const wxVector<wxColumnChartDataset::ptr>& datasets)
{
	wxDouble result = 0;
	bool foundValue = false;

	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxVector<wxDouble>& values = datasets[i]->GetData();
		for (size_t j = 0; j < values.size(); ++j)
		{
			if (!foundValue)
			{
				result = values[j];
				foundValue = true;
			}
			else if (result > values[j])
			{
				result = values[j];
			}
		}
	}

	return result;
}

wxDouble wxColumnChartCtrl::GetMaxValue(const wxVector<wxColumnChartDataset::ptr>& datasets)
{
	wxDouble result = 0;
	bool foundValue = false;

	for (size_t i = 0; i < datasets.size(); ++i)
	{
		const wxVector<wxDouble>& values = datasets[i]->GetData();
		for (size_t j = 0; j < values.size(); ++j)
		{
			if (!foundValue)
			{
				result = values[j];
				foundValue = true;
			}
			else if (result < values[j])
			{
				result = values[j];
			}
		}
	}

	return result;
}

void wxColumnChartCtrl::Resize(const wxSize &size)
{
	m_grid.Resize(size);
}

wxSharedPtr<wxVector<const wxChartElement*> > wxColumnChartCtrl::GetActiveElements(const wxPoint &point)
{
	wxSharedPtr<wxVector<const wxChartElement*> > activeElements(new wxVector<const wxChartElement*>());
	return activeElements;
}

void wxColumnChartCtrl::OnPaint(wxPaintEvent &evt)
{
	wxAutoBufferedPaintDC dc(this);

	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
	if (gc)
	{
		m_grid.Draw(*gc);

		for (size_t i = 0; i < m_datasets.size(); ++i)
		{
			Dataset& currentDataset = *m_datasets[i];
			for (size_t j = 0; j < currentDataset.GetColumns().size(); ++j)
			{
				Column& column = *(currentDataset.GetColumns()[j]);
				wxPoint2DDouble position = m_grid.GetMapping().GetWindowPosition(j, column.GetValue());
				column.SetPosition(position);
				column.SetSize(20, m_grid.GetMapping().GetEndPoint() - position.m_y);
			}
		}

		for (size_t i = 0; i < m_datasets.size(); ++i)
		{
			Dataset& currentDataset = *m_datasets[i];
			for (size_t j = 0; j < currentDataset.GetColumns().size(); ++j)
			{	
				currentDataset.GetColumns()[j]->Draw(*gc);
			}
		}

		delete gc;
	}
}

BEGIN_EVENT_TABLE(wxColumnChartCtrl, wxChartCtrl)
	EVT_PAINT(wxColumnChartCtrl::OnPaint)
END_EVENT_TABLE()